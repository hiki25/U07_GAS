#include "CWarpBall.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACWarpBall::ACWarpBall()
{
	DetonateDelay = TeleportDelay =  0.2f;

	MoveComp->InitialSpeed = 6000.f;
}

void ACWarpBall::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimeHandle_DelayDetonate, this, &ACWarpBall::Explode, DetonateDelay);
}

void ACWarpBall::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimeHandle_DelayDetonate);

	UGameplayStatics::SpawnEmitterAtLocation(this,ImpactVFX,GetActorLocation(),GetActorRotation());

	EffectComp->DeactivateSystem();
	MoveComp->StopMovementImmediately();

	SetActorEnableCollision(false);

	FTimerHandle TimeHandle_Teleport;
	GetWorldTimerManager().SetTimer(TimeHandle_Teleport, this, &ACWarpBall::TeleportInstigator, TeleportDelay);
}

void ACWarpBall::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation());
	}

	Destroy();
}
