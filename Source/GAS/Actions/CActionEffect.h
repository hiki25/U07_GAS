#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "Engine.h"
#include "CActionEffect.generated.h"

UCLASS()
class GAS_API UCActionEffect : public UCAction
{
	GENERATED_BODY()

public:
	UCActionEffect();

public:
	void StartAction_Implementation(AActor* Instigator) override;
	void StopAction_Implementation(AActor* Instigator) override;

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodEffect(AActor* Instigator);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect")
		float Duration;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect")
		float Period;

	FTimerHandle DurationHandle;
	FTimerHandle PeriodHandle;

};
