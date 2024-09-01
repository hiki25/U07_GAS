#pragma once

#include "CoreMinimal.h"
#include "Interaction/CPickupBase.h"
#include "CWarpCow.generated.h"

class UCAction;

UCLASS()
class GAS_API ACWarpCow : public ACPickupBase
{
	GENERATED_BODY()
	
public:
	ACWarpCow();

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	TSubclassOf<UCAction> ActionClass;

};
