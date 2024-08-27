#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCInteractionComponent;
class UAnimMontage;
class UCAttributeComponent;
class UParticleSystem;
class UCActionComponent;

UCLASS()
class GAS_API ACPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ACPlayer();

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);

protected:
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	 FVector GetPawnViewLocation() const override;

protected:
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewHealth, float Delta);

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void PrimaryAction();

	void SecondaryAction();

	void ThirdAction();

	void PrimaryInteraction();

	void StartSprint();
	void StopSprint();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCInteractionComponent* InteractionComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UCAttributeComponent* AttributeComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UCActionComponent* ActionComp;

protected:





	UPROPERTY(EditAnywhere, Category = "Action")
		FName TimeToHitParamName;
};
