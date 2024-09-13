#include "CWorldWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"

void  UCWorldWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FVector2D ScreenPosition;
	bool bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachToActor->GetActorLocation() + WorldOffset, ScreenPosition);
	if (AttachToActor == nullptr)
	{
		return;
	}
	if (bIsOnScreen)
	{
		float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition /= ViewportScale;

		if (ParentSizeBox)
		{
		ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
	if (ParentSizeBox)
	{
	ParentSizeBox->SetVisibility(bIsOnScreen ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}