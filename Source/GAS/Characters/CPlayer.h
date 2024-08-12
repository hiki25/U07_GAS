#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCInteractionComponent;
class UAnimMontage;
class UCAttributeComponent;

UCLASS()
class GAS_API ACPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void PrimaryAction();
	void PrimaryAction_TimeElapsed();

	void PrimaryInteraction();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCInteractionComponent* InteractionComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UCAttributeComponent* AttributeComp;


protected:
	UPROPERTY(EditAnywhere, Category = "Action")
		float AttackDelay;

	UPROPERTY(EditAnywhere, Category = "Action")
	TSubclassOf<AActor> MagicBallClass;

	UPROPERTY(EditAnywhere, Category = "Action")
	UAnimMontage* AttackMontage;

private:
	FTimerHandle TimerHandle_PrimaryAction;
};
