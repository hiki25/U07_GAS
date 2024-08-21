#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "CprojectileBase.generated.h"

class USphereComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;
class UAudioComponent;
class USoundCue;
class UCameraShake;

UCLASS()
class GAS_API ACprojectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ACprojectileBase();

protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
		virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Explode();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effects")
		UParticleSystem* ImpactVFX;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		USoundCue* ImpactSound;
	UPROPERTY(EditDefaultsOnly, Category = "Effects | Shake")
		TSubclassOf<UCameraShake> ImpactShake;

	UPROPERTY(EditDefaultsOnly, Category = "Effects | Shake")
		float ImpactShakeInnerRadius;
	UPROPERTY(EditDefaultsOnly, Category = "Effects | Shake")
		float ImpactShakeOuterRadius;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Component")
		USphereComponent* SphereComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Component")
		UParticleSystemComponent* EffectComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Component")
		UProjectileMovementComponent* MoveComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
		UAudioComponent* AudioComp;


	

};
