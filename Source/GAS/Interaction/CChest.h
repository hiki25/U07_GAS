#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/CGameplayInterface.h"
#include "CChest.generated.h"

UCLASS()
class GAS_API ACChest : public AActor, public ICGameplayInterface
{
	GENERATED_BODY()
	
public:	
	ACChest();

protected:
	virtual void BeginPlay() override;

public:
	void Interact_Implementation(APawn* InstigatorPawn) override;

public:
	UPROPERTY(EditAnywhere)
		float MaxPitch;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UStaticMeshComponent* LidMesh;


};
