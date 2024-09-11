#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine.h"
#include "Engine/DataTable.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "CGameMode.generated.h"

class UEnvQuery;
class UCSaveGame;
class UCSpawnBotDataAsset;



USTRUCT(BlueprintType)
struct FSpawnBotRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	FSpawnBotRow()
	{
		weight = 1.f;
		Cost = 5.f;
		KillReward = 20.f;
	}

public:

	UPROPERTY(EditAnywhere, Category = "AI")
	UCSpawnBotDataAsset* SpawnBotDataAsset;

	UPROPERTY(EditAnywhere, Category = "AI")
	float weight;

	UPROPERTY(EditAnywhere, Category = "AI")
	float Cost;

	UPROPERTY(EditAnywhere, Category = "AI")
	float KillReward;
};

UCLASS()
class GAS_API ACGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACGameMode();

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;
	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

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
	UCurveFloat* SpawnCurve;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		UDataTable* SpawnBotDataTable;

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

//SaveGame
protected:
	UPROPERTY()
	UCSaveGame* CurrentSaveGame;

	FString SlotName;

public:
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();


};
