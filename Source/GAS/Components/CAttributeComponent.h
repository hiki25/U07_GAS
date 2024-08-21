#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, class UCAttributeComponent*, OwningComp,float, NewHealth ,float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_API UCAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCAttributeComponent();

protected:
	virtual void BeginPlay() override;


public:
	//체력에 대한 변화량
	UFUNCTION(BlueprintCallable, Category = "Attribute")
	bool ApplyHealthChange(float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
		bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	bool IsFullHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	float GetMaxHealth() const;

public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Attribute")
	float Health;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Attribute")
	float MaxHealth;
		
};
