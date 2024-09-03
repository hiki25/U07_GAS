#include "CActionEffect.h"
#include "Components/CActionComponent.h"

UCActionEffect::UCActionEffect()
{
	bAutoStart = true;
}

void UCActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.f)
	{
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "StopAction", Instigator);

	GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate,Duration,false);
	}
	if (Period > 0.f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}


void UCActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodEffect(Instigator);
	}

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	Super::StopAction_Implementation(Instigator);

	UCActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		Comp->RemoveAction(this);
	}
}

void UCActionEffect::ExecutePeriodEffect_Implementation(AActor* Instigator)
{
	//
}
