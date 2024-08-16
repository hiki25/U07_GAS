#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CBot.generated.h"

class UPawnSensingComponent;

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

protected:
	UPROPERTY(VisibleDefaultsOnly)
	UPawnSensingComponent* PawnSensingComp;


};
