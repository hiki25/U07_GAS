#include "CTargetDummy.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CAttributeComponent.h"

ACTargetDummy::ACTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<UCAttributeComponent>("Attribute");
}

void ACTargetDummy::BeginPlay()
{
	Super::BeginPlay();
	
	AttributeComp->OnHealthChanged.AddDynamic(this, &ACTargetDummy::OnHealthChanged);

}

void ACTargetDummy::OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}

