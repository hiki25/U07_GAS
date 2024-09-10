#include "CWorldWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"

void  UCWorldWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


	FVector2D ScreenPosition;
	if (AttachToActor == nullptr)
	{
		return;
	}
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachToActor->GetActorLocation() + WorldOffset, ScreenPosition))
	{
		float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition /= ViewportScale;

		if (ParentSizeBox)
		{
		ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}