#include "CGameMode.h"
#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Characters/CBot.h"
#include "Characters/CPlayer.h"
#include "Components/CAttributeComponent.h"
#include "DrawDebugHelpers.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("Tore.SpawnBots"), true, TEXT("Enable spawn bots via cvar"), ECVF_Cheat);

ACGameMode::ACGameMode()
{
	SpawnTimerDelay = 2.0f;
}

void ACGameMode::StartPlay()
{
	Super::StartPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBot,this,&ACGameMode::SpawnBotTimerElapsed,SpawnTimerDelay,true);
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
		UE_LOG(LogTemp, Log, TEXT("Disabled bot spawning via CVar"));
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
	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ACGameMode::OnQueryFinished);
	}
}

void ACGameMode::OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
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
