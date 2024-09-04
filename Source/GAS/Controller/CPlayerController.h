#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);

UCLASS()
class GAS_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetPawn(APawn* InPawn) override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;
	
};
