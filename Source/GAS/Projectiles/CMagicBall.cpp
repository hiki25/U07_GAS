#include "CMagicBall.h"
#include "Components/SphereComponent.h"
#include "Game/CFunctionLibrary.h"
#include "Components/CActionComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACMagicBall::ACMagicBall()
{
	SphereComp->SetSphereRadius(20);
	DamageAmount = 20.f;
}

void ACMagicBall::BeginPlay()
{
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACMagicBall::OnActorOverlap);

	Super::BeginPlay();
}

void ACMagicBall::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		UCActionComponent* ActionComp = Cast<UCActionComponent>(OtherActor->GetComponentByClass(UCActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ReflectTag))
		{
			MoveComp->Velocity = -(MoveComp->Velocity);
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
		if (UCFunctionLibrary::ApplyDirectionDamage(GetInstigator(), OtherActor,DamageAmount, SweepResult))
		{
			Explode();
		}
	}
}

