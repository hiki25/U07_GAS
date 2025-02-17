#include "CGameMode.h"
#include "EngineUtils.h"
#include "Engine/AssetManager.h"
#include "GameFramework/GameStateBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Characters/CBot.h"
#include "Characters/CPlayer.h"
#include "Components/CAttributeComponent.h"
#include "Components/CActionComponent.h"
#include "DrawDebugHelpers.h"
#include "CPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "CSaveGame.h"
#include "CGameplayInterface.h"
#include "CSpawnBotDataAsset.h"
#include "GAS.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("Tore.SpawnBots"), true, TEXT("Enable spawn bots via cvar"), ECVF_Cheat);

ACGameMode::ACGameMode()
{
	SpawnTimerDelay = 2.0f;
	CreditsPerKill = 20;

	MinimumPickupDistance = 2000.f;
	MaxPickupCount = 10;

	PlayerStateClass = ACPlayerState::StaticClass();

	SlotName = "Game01";

}

void ACGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString ParseValue = UGameplayStatics::ParseOption(Options,"savegame");
	if (ParseValue.Len() > 0)
	{
		SlotName = ParseValue;
	}

	LoadSaveGame();
}

void ACGameMode::StartPlay()
{
	Super::StartPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBot,this,&ACGameMode::SpawnBotTimerElapsed,SpawnTimerDelay,true);

	if (ensure(PickupClassess.Num() > 0))
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this,SpawnPickupQuery,this,EEnvQueryRunMode::AllMatching,nullptr);
		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ACGameMode::OnSpawnPickUpQueryFinished);
		}
	}
}

void ACGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{

	ACPlayerState* PS = NewPlayer->GetPlayerState<ACPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void ACGameMode::KillAll()
{
	for (TActorIterator<ACBot> It(GetWorld()); It; ++It)
	{
		ACBot* Bot = *It;
		UCAttributeComponent* AttributeComp = UCAttributeComponent::GetAttribute(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this);
		}
	}
}

void ACGameMode::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ACPlayer* Player = Cast<ACPlayer>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"ReSpawnPlayerElapsed",Player->GetController());

		float RespawnDelay = 5.f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);

		Player->SetLifeSpan(3.f);
	}

	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn && KillerPawn != VictimActor)
	{
		ACPlayerState* PS = KillerPawn->GetPlayerState<ACPlayerState>();
		if (PS)
		{
			PS->AddCredits(CreditsPerKill);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled, Victim : %s, Killer : %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}

void ACGameMode::ReSpawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void ACGameMode::SpawnBotTimerElapsed()
{

	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		//UE_LOG(LogTemp, Log, TEXT("Disabled bot spawning via CVar"));
		return;
	}

	int32 NumOfAliveBots = 0;
	for (TActorIterator<ACBot> It(GetWorld()); It; ++It)
	{
		ACBot* Bot = *It;

		UCAttributeComponent* AttributeComp = UCAttributeComponent::GetAttribute(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NumOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), NumOfAliveBots);

	float MaxBotCount = 10.f;
	if (SpawnCurve)
	{
		MaxBotCount = SpawnCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NumOfAliveBots >= (int32)MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("Reached Maximum bot count. Skipping spawn bot"));
		return;
	}


	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotEQS, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ACGameMode::OnSpawnBotQueryFinished);
	}
}

void ACGameMode::OnSpawnBotQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("EQS Failed"));
		return;
	}

	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		if (SpawnBotDataTable)
		{
			TArray<FSpawnBotRow*> Rows;
			SpawnBotDataTable->GetAllRows("", Rows);

			int32 Random = FMath::RandRange(0, Rows.Num() - 1);
			FSpawnBotRow* SelectRow = Rows[Random];

			UAssetManager* AssetManager = UAssetManager::GetIfValid();
			if (AssetManager)
			{
				LogOnScreen(this, "Loading Spawn Bot Data Asset...", FColor::Yellow);

				FTransform Transform;
				Transform.SetLocation(Locations[0]);

				TArray<FName> Bundles;
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ACGameMode::OnDataAssetLoaded, SelectRow->BotDataAssetID, Transform);

				AssetManager->LoadPrimaryAsset(SelectRow->BotDataAssetID, Bundles, Delegate);
			}
		}
	}

}

void ACGameMode::OnDataAssetLoaded(FPrimaryAssetId PrimaryDataAssetID, FTransform Transform)
{
	LogOnScreen(this, PrimaryDataAssetID.PrimaryAssetName.ToString() + "Is Loaded", FColor::Blue);

	UAssetManager* AssetManager = UAssetManager::GetIfValid();
	if (AssetManager)
	{
		UCSpawnBotDataAsset* DataAsset = Cast<UCSpawnBotDataAsset>(AssetManager->GetPrimaryAssetObject(PrimaryDataAssetID));
		if (DataAsset)
		{
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(DataAsset->BotClass, Transform);

			ACBot* NewBot = Cast<ACBot>(NewActor);
			if (NewBot)
			{
				NewBot->SetBodyColor(DataAsset->BotColor);

				UCActionComponent* ActionComp = Cast<UCActionComponent>(NewBot->GetComponentByClass(UCActionComponent::StaticClass()));
				if (ActionComp)
				{

					for (TSubclassOf<UCAction> Action : DataAsset->Actions)
					{
						ActionComp->AddAction(NewBot, Action);
					}

				}

				NewBot->FinishSpawning(Transform);
			}
		}
	}

	
}


void ACGameMode::OnSpawnPickUpQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("SPawn PickUp Query failed"));
		return;
	}

	 TArray<FVector> EQSLocation =  QueryInstance->GetResultsAsLocations();

	 TArray<FVector> UsedLocations;

	 int32 SpawnCount = 0;

	 while (SpawnCount < MaxPickupCount && EQSLocation.Num() > 0)
	 {
		 int32 RandomLocationIndex = FMath::RandRange(0, EQSLocation.Num() - 1);
		 FVector SelectedLocation = EQSLocation[RandomLocationIndex];

		 EQSLocation.RemoveAt(RandomLocationIndex);

		 bool bValidLocation = true;
		 for (FVector UsedLoaction : UsedLocations)
		 {
			 float Distance = (SelectedLocation - UsedLoaction).Size();
			 if (Distance < MinimumPickupDistance)
			 {
				 DrawDebugSphere(GetWorld(),SelectedLocation, 50.f,20,FColor::Yellow,false,10.f);

				 bValidLocation = false;
				 break;
			 }
			 
		 }

		 if (!bValidLocation)
		 {
			 continue;
		 }

		 int32 RandomClassIndex = FMath::RandRange(0, PickupClassess.Num() - 1);
		 TSubclassOf<AActor> SelectedClass = PickupClassess[RandomClassIndex];
		 SelectedLocation.Z += 40.f;
		 GetWorld()->SpawnActor<AActor>(SelectedClass, SelectedLocation,FRotator::ZeroRotator);

		 UsedLocations.Add(SelectedLocation);
		 SpawnCount++;
	 }
}

void ACGameMode::WriteSaveGame()
{
	//PlayStates, Credit
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		ACPlayerState* PS = Cast<ACPlayerState>(GameState->PlayerArray[i]); 
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break;
		}
	}

		//Actor
	CurrentSaveGame->SavedActors.Empty();

	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->Implements<UCGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetTransform();

		//직렬화에 쓸 버퍼의 크기, 용도(쓰기)를 정하는 구간
		//버퍼를 메모리에 올린 후 설정
		//직렬화
		FMemoryWriter MemWriter(ActorData.ByteData);
		FObjectAndNameAsStringProxyArchive Desc(MemWriter,true);
		Desc.ArIsSaveGame = true; //SaveGame 옵션이 켜져있는 변수를 찾아라
		Actor->Serialize(Desc);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}
	

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame,SlotName,0);
}

void ACGameMode::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UCSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName,0));

		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed Load SaveGame Data."));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));

		//Load Actor
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if (!Actor->Implements<UCGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (Actor->GetName() == ActorData.ActorName)
				{
					Actor->SetActorTransform(ActorData.Transform);

					//메모리를읽는 버퍼
					FMemoryReader MemReader(ActorData.ByteData);
					FObjectAndNameAsStringProxyArchive Desc(MemReader, true);
					Desc.ArIsSaveGame = true; //SaveGame 옵션이 켜져있는 변수를 찾아라
					Actor->Serialize(Desc);

					ICGameplayInterface::Execute_OnActorLoaded(Actor);

					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<UCSaveGame>(UGameplayStatics::CreateSaveGameObject(UCSaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Created new SaveGame Data."));
	}
}
