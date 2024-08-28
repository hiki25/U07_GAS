#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "CGameMode.generated.h"

class UEnvQuery;

UCLASS()
class GAS_API ACGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACGameMode();

protected:
	virtual void StartPlay() override;

	UFUNCTION(Exec)
		void KillAll();

	UFUNCTION()
		void ReSpawnPlayerElapsed(AController* Controller);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Credits")
	int32 CreditsPerKill;

public:
	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	//Spawn Bots
protected:
	FTimerHandle TimerHandle_SpawnBot;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
		void OnSpawnBotQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotEQS;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerDelay;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> BotClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* SpawnCurve;

	//Spawn PickUp

protected:
	UPROPERTY(EditDefaultsOnly, Category = "PickUp")
	UEnvQuery* SpawnPickupQuery;

	UPROPERTY(EditDefaultsOnly, Category = "PickUp")
	TArray<TSubclassOf<AActor>> PickupClassess;

	UPROPERTY(EditDefaultsOnly, Category = "PickUp")
	float MinimumPickupDistance;

	UPROPERTY(EditDefaultsOnly, Category = "PickUp")
	int32 MaxPickupCount;

	UFUNCTION()
	void OnSpawnPickUpQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
};
