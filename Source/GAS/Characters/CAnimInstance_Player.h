#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance_Player.generated.h"

class UCActionComponent;

UCLASS()
class GAS_API UCAnimInstance_Player : public UAnimInstance
{
	GENERATED_BODY()
	

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	UCActionComponent* ActionComp;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	bool bIsStunned;
};
