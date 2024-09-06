#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CPlayerState.generated.h"

class UCSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ACPlayerState* , PlayerState, int32, NewCredits, int32, Delta);

UCLASS()
class GAS_API ACPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits() const;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Delta);

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(UCSaveGame* SaveGame);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(UCSaveGame* SaveGame);

public:
	UPROPERTY(BlueprintAssignable, Category = "Credit")
	FOnCreditsChanged OnCreditsChanged;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Credits", ReplicatedUsing = "OnRep_Credit")
	int32 Credits;

	UFUNCTION()
	void OnRep_Credit(int32 OldCredits);
};
