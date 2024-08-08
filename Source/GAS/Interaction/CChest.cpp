#include "CChest.h"
#include "Components/StaticMeshComponent.h"

ACChest::ACChest()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);
	LidMesh->SetRelativeLocation(FVector(-35.f, 0.0f, 50.f));

	MaxPitch = 110.f;
}

void ACChest::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(MaxPitch, 0, 0));
}
