#include "CAction.h"

bool UCAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	UCActionComponent* ActionComp = GetOwningComponent();

	if (ActionComp->ActiveGameplayTags.HasAny(BlockTags))
	{
		return false;
	}

	return true;
}

void UCAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running : %s"), *GetNameSafe(this));


	UCActionComponent* ActionComp = GetOwningComponent();
	ActionComp->ActiveGameplayTags.AppendTags(GrantTags);

	bIsRunning = true;
}

void UCAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopping : %s"), *GetNameSafe(this));

	

	UCActionComponent* ActionComp = GetOwningComponent();
	ActionComp->ActiveGameplayTags.RemoveTags(GrantTags);

	bIsRunning = false;
}



UWorld* UCAction::GetWorld() const
{
	UCActionComponent* ActionComp = Cast<UCActionComponent>(GetOuter());
	if (ActionComp)
	{
		return ActionComp->GetWorld();
	}
	return nullptr;
}

UCActionComponent* UCAction::GetOwningComponent() const
{
	return Cast<UCActionComponent>(GetOuter());
}

bool UCAction::IsRunning() const
{
	return bIsRunning;
}