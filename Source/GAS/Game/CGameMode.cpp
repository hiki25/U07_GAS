#include "CGameMode.h"
#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Characters/CBot.h"
#include "Components/CAttributeComponent.h"

ACGameMode::ACGameMode()
{
	SpawnTimerDelay = 2.0f;
}

void ACGameMode::StartPlay()
{
	Super::StartPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBot,this,&ACGameMode::SpawnBotTimerElapsed,SpawnTimerDelay,true);
}

void ACGameMode::SpawnBotTimerElapsed()
{
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

	int32 NumOfAliveBots = 0;
	for(TActorIterator<ACBot> It(GetWorld()); It; ++It)
	{
		ACBot* Bot = *It;

		UCAttributeComponent* AttributeComp = Cast<UCAttributeComponent>(Bot->GetComponentByClass(UCAttributeComponent::StaticClass()));
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NumOfAliveBots++;
		}
	}

	float MaxBotCount = 10.f;
	if (SpawnCurve)
	{
		MaxBotCount = SpawnCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NumOfAliveBots >= (int32)MaxBotCount)
	{
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
	}

}
