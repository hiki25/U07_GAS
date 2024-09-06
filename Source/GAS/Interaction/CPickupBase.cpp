#include "CPickupBase.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

ACPickupBase::ACPickupBase()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Pickup");

	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	ReSpawnTime = 10.f;

	bIsActive = true;

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
	bIsActive = bNewActive;
	OnRep_IsActive();
}

void ACPickupBase::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	RootComponent->SetVisibility(bIsActive,true);
}

void ACPickupBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPickupBase, bIsActive);
}