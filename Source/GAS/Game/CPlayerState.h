#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CPlayerState.generated.h"

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

public:
	UPROPERTY(BlueprintAssignable, Category = "Credit")
	FOnCreditsChanged OnCreditsChanged;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 Credits;
};
