#include "CAnimInstance_Player.h"
#include "Components/CActionComponent.h"

void UCAnimInstance_Player::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwningActor = GetOwningActor();
	if (OwningActor)
	{
		ActionComp = Cast<UCActionComponent>(OwningActor->GetComponentByClass(UCActionComponent::StaticClass()));
	}
}

void UCAnimInstance_Player::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");

	if (ActionComp)
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
