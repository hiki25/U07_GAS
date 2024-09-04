#pragma once

#include "CoreMinimal.h"
#include "Interaction/CPickupBase.h"
#include "CPickupBase_AddAction.generated.h"

class UCAction;

UCLASS()
class GAS_API ACPickupBase_AddAction : public ACPickupBase
{
	GENERATED_BODY()
	
public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	TSubclassOf<UCAction> ActionToGrant;

};
