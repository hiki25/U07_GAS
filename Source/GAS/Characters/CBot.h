#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CBot.generated.h"

class UPawnSensingComponent;
class UCAttributeComponent;
class UCWorldWidget;
class UCActionComponent;

UCLASS()
class GAS_API ACBot : public ACharacter
{
	GENERATED_BODY()

public:
	ACBot();

protected:
	virtual void PostInitializeComponents() override;

protected:
	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	UFUNCTION()
		void OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewHealth, float Delta);

	void SetTargetActor(AActor* NewTarget);
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Component")
	UCAttributeComponent* AttributeComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
	UCActionComponent* ActionComp;

	UPROPERTY(EditDefaultsOnly, Category = "impact")
	FName TimeToHitParamName;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UCWorldWidget> HealthBarWidgetClass;

	UCWorldWidget* HealthBarWidget;
};
