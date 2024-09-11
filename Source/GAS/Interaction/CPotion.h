#pragma once

#include "CoreMinimal.h"
#include "Interaction/CPickupBase.h"
#include "CPotion.generated.h"


UCLASS()
class GAS_API ACPotion : public ACPickupBase
{
	GENERATED_BODY()

public:
	ACPotion();

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 CreditCost;

	UPROPERTY(EditAnywhere, Category = "Credits")
		FText WarningMsg;

	UPROPERTY(EditAnywhere, Category = "Credits")
		FText InteractionMsg;
	
};
