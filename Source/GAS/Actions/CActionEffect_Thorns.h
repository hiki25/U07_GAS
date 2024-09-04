#pragma once

#include "CoreMinimal.h"
#include "Actions/CActionEffect.h"
#include "CActionEffect_Thorns.generated.h"

class UCAttributeComponent;

UCLASS()
class GAS_API UCActionEffect_Thorns : public UCActionEffect
{
	GENERATED_BODY()

public:
	UCActionEffect_Thorns();

public:
	void StartAction_Implementation(AActor* Instigator) override;
	void StopAction_Implementation(AActor* Instigator) override;

protected:
	UFUNCTION()
		void OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewValue, float Delta);

	UPROPERTY(EditDefaultsOnly, Category = "Thorn", meta = (ClampMax = 1.00, ClampMin = 0.00))
	float ReflectRatio;
	
};
