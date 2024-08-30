#include "CInteractionComponent.h"
#include "Engine.h"
#include "DrawDebugHelpers.h"
#include "Game/CGameplayInterface.h"
#include "UI/CWorldWidget.h"

static TAutoConsoleVariable<bool> CVarDrawDebug(TEXT("Tore.DrawDebug"), false, TEXT("Enable DrawDebug for Interaction"));

UCInteractionComponent::UCInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceDistance = 500.f;
	TraceRadius = 30.f;
	CollisionChannel = ECollisionChannel::ECC_WorldDynamic;


}


void UCInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn->IsLocallyControlled())
	{
	FindNearestInteractable();
	}
	
}

void UCInteractionComponent::FindNearestInteractable()
{
	bool bDrawDebug = CVarDrawDebug.GetValueOnGameThread();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* OwnerActor = GetOwner();
	FVector EyeLocation;
	FRotator EyeRotation;
	OwnerActor->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	TArray<FHitResult> Hits;

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Red : FColor::Green;
	FocusedActor = nullptr;


	for (const auto& Hit : Hits)
	{
		if (bDrawDebug)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 20, LineColor, false, 0.f);
		}

		AActor* HitActor = Hit.GetActor();

		if (HitActor)
		{
			if (HitActor->Implements<UCGameplayInterface>())
			{
				FocusedActor = HitActor;

				break;
			}
		}

	}
	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<UCWorldWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachToActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}

	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false , 0.f, 0, 2.f);
	}
}

void UCInteractionComponent::PrimaryInteraction()
{
	ServerInteract(FocusedActor);
}

void UCInteractionComponent::ServerInteract_Implementation(AActor* InFocused)
{
	if (InFocused == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "NO INTERACTION ACTOR");
		return;
	}
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	ICGameplayInterface::Execute_Interact(InFocused, OwnerPawn);
}
