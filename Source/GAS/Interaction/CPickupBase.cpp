#include "CPickupBase.h"
#include "Components/SphereComponent.h"

ACPickupBase::ACPickupBase()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Pickup");

	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	ReSpawnTime = 10.f;

	SetReplicates(true);
}

void ACPickupBase::Interact_Implementation(APawn* InstigatorPawn)
{
	
}

void ACPickupBase::Show()
{
	SetPickupState(true);
}

void ACPickupBase::HideAndCooldown()
{
	SetPickupState(false);

	GetWorldTimerManager().SetTimer(TimerHandle_ReSpawnTimer,this, &ACPickupBase::Show,ReSpawnTime);
}

void ACPickupBase::SetPickupState(bool bNewActive)
{
	SetActorEnableCollision(bNewActive);
	RootComponent->SetVisibility(bNewActive,true);
}
