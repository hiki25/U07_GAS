#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBarrel_Hit.generated.h"

class UStaticMeshComponent;
class URadialForceComponent;
class UParticleSystem;

UCLASS()
class GAS_API ACBarrel_Hit : public AActor
{
	GENERATED_BODY()
	
public:	
	ACBarrel_Hit();

protected:
	void PostInitializeComponents() override;

private:
	UFUNCTION()
		void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		URadialForceComponent* ForceComp;

	UPROPERTY(EditAnywhere, Category = "Component")
		UParticleSystem* Explosion;
};
