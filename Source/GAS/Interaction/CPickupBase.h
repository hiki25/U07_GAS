#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/CGameplayInterface.h"
#include "Engine.h"
#include "CPickupBase.generated.h"

class USphereComponent;

UCLASS(Abstract)
class GAS_API ACPickupBase : public AActor, public ICGameplayInterface
{
	GENERATED_BODY()
	
public:	
	ACPickupBase();

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UFUNCTION()
	void Show();
	void HideAndCooldown();

	void SetPickupState(bool bNewActive);

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Pickup")
		float ReSpawnTime;

	FTimerHandle TimerHandle_ReSpawnTimer;

	UPROPERTY(ReplicatedUsing = "OnRep_IsActive")
	bool bIsActive;

	UFUNCTION()
	void OnRep_IsActive();
};
