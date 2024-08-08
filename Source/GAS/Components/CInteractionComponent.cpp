#include "CInteractionComponent.h"
#include "Engine.h"
#include "DrawDebugHelpers.h"

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

	FColor LinearColor = bBlockingHit ? FColor::Red : FColor::Green;
	DrawDebugLine(GetWorld(), EyeLocation, End, LinearColor, false, 2.f, 0, 2.f);
}
