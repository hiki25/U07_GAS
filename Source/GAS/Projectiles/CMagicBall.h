#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CprojectileBase.h"
#include "GameplayTagContainer.h"
#include "CMagicBall.generated.h"

UCLASS()
class GAS_API ACMagicBall : public ACprojectileBase
{
	GENERATED_BODY()
	
public:	
	ACMagicBall();

protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ReflectTag;
};
