#include "CGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/GameStateBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Characters/CBot.h"
#include "Characters/CPlayer.h"
#include "Components/CAttributeComponent.h"
#include "DrawDebugHelpers.h"
#include "CPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "CSaveGame.h"
#include "CGameplayInterface.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("Tore.SpawnBots"), false, TEXT("Enable spawn bots via cvar"), ECVF_Cheat);

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
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	ACPlayerState* PS = NewPlayer->GetPlayerState<ACPlayerState>();
	if (PS)
	{
		PS->SavePlayerState(CurrentSaveGame);
	}
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
	if (KillerPawn)
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

		if (!ensure(BotClass))
		{
			UE_LOG(LogTemp, Error, TEXT("Botclass Is Not Set"));
			return;
		}
		GetWorld()->SpawnActor<AActor>(BotClass, Locations[0],FRotator::ZeroRotator);
		DrawDebugSphere(GetWorld(), Locations[0],50.f,20, FColor::Blue,false,60.f);
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
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		ACPlayerState* PS = Cast<ACPlayerState>(GameState->PlayerArray[i]); 
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break;
		}

		//Actor
		for (FActorIterator It(GetWorld()); It; It++)
		{
			AActor* Actor = *It;
			if (!Actor->Implements<UCGameplayInterface>())
			{
				continue;
			}
		}
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
	}
	else
	{
		CurrentSaveGame = Cast<UCSaveGame>(UGameplayStatics::CreateSaveGameObject(UCSaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Created new SaveGame Data."));
	}
}
