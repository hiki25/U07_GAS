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

protected:
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewHealth, float Delta);

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void PrimaryAction();
	void PrimaryAction_TimeElapsed();

	void SecondaryAction();
	void SecondaryAction_TimeElapsed();

	void ThirdAction();
	void ThirdAction_TimeElapsed();

	void PlayActtackAction();

	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

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

	UPROPERTY(EditAnywhere, Category = "Action")
		UParticleSystem* MuzzleParticle;

	UPROPERTY(EditAnywhere, Category = "Action")
		FName HandSocketName;;

protected:
	UPROPERTY(EditAnywhere, Category = "Action")
		float AttackDelay;

	UPROPERTY(EditAnywhere, Category = "Action")
	TSubclassOf<AActor> MagicBallClass;

	UPROPERTY(EditAnywhere, Category = "Action")
		TSubclassOf<AActor> WarpBallClass;

	UPROPERTY(EditAnywhere, Category = "Action")
		TSubclassOf<AActor> BlackHoleClass;

	UPROPERTY(EditAnywhere, Category = "Action")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Action")
		FName TimeToHitParamName;

private:
	FTimerHandle TimerHandle_PrimaryAction;
	FTimerHandle TimerHandle_SecondartAction;
	FTimerHandle TimerHandle_ThirdAction;
};
