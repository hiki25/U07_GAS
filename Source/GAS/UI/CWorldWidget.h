#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWorldWidget.generated.h"

class USizeBox;

UCLASS()
class GAS_API UCWorldWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	AActor* AttachToActor;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
		FVector WorldOffset;

protected:
	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;

};
