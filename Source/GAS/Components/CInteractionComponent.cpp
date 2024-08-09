#include "CInteractionComponent.h"
#include "Engine.h"
#include "DrawDebugHelpers.h"
#include "Game/CGameplayInterface.h"

UCInteractionComponent::UCInteractionComponent()
{
}


void UCInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCInteractionComponent::PrimaryInteraction()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* OwnerActor = GetOwner();
	FVector EyeLocation;
	FRotator EyeRotation;
	OwnerActor->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);

	TArray<FHitResult> Hits;

	float Radius = 30.f;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits,EyeLocation,End,FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Red : FColor::Green;

	for (const auto& Hit : Hits)
	{
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 20, LineColor, false, 3.f);

		AActor* HitActor = Hit.GetActor();

		if (HitActor)
		{
			if (HitActor->Implements<UCGameplayInterface>())
			{
				APawn* OwnerPawn = Cast<APawn>(OwnerActor);
				ICGameplayInterface::Execute_Interact(HitActor, OwnerPawn);
			}
		}

	}
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.f, 0, 2.f);
}
