#pragma once

#include "CoreMinimal.h"
#include "Interaction/CPickupBase.h"
#include "CCredit.generated.h"

UCLASS()
class GAS_API ACCredit : public ACPickupBase
{
	GENERATED_BODY()

public:
	ACCredit();

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Credits")
		int32 CreditAmount;
	
};
