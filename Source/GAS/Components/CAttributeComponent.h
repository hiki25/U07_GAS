#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine.h"
#include "CAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, class UCAttributeComponent*, OwningComp,float, NewHealth ,float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChargehChanged ,float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_API UCAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCAttributeComponent();

protected:
	virtual void BeginPlay() override;


public:
	UFUNCTION(BlueprintCallable, Category = "Attribute")
	static UCAttributeComponent* GetAttribute(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attribute", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);


	//체력에 대한 변화량
	UFUNCTION(BlueprintCallable, Category = "Attribute")
	bool ApplyHealthChange(AActor* Instigatorctor,float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	bool IsFullHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attribute")
		bool IsFullCharge() const;


	UFUNCTION(BlueprintCallable, Category = "Attribute")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	float GetMaxCharge() const;

	UFUNCTION(BlueprintCallable, Category = "Attribute")
		void  PlusCharge(float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Cheat")
	bool Kill(AActor* Killer);

	void UsedCharge();


public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnChargehChanged OnChargehChanged;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Attribute")
	float Health;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Attribute")
	float MaxHealth;

protected:
	//궁극기
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Attribute")
		float MaxCharge;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Attribute")
		float Charge;
		
};
