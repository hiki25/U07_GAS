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

	//Spawn Bots
protected:
	FTimerHandle TimerHandle_SpawnBot;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
		void OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotEQS;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerDelay;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> BotClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* SpawnCurve;
};
