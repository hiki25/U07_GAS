#pragma once

#include "CoreMinimal.h"
#include "Interaction/CPickupBase.h"
#include "CPotion.generated.h"

class UStaticMeshComponent;

UCLASS()
class GAS_API ACPotion : public ACPickupBase
{
	GENERATED_BODY()

public:
	ACPotion();

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UStaticMeshComponent* MeshComp;


	
};
