#include "CPlayer.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CAttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CInteractionComponent.h"
#include "Components/CActionComponent.h"

ACPlayer::ACPlayer()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");

	InteractionComp = CreateDefaultSubobject<UCInteractionComponent>("InteractionComp");
	AttributeComp = CreateDefaultSubobject<UCAttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<UCActionComponent>("ActionComp");

	//Component Attach
	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp->SetupAttachment(SpringArmComp);

	//SpringArmComp Setting
	SpringArmComp->bUsePawnControlRotation = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	TimeToHitParamName = "TimeToHit";
}

void ACPlayer::HealSelf(float Amount)
{
	AttributeComp->ApplyHealthChange(this,Amount);
}

void ACPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this,&ACPlayer::OnHealthChanged);
	
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Axix Bind
	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	//Action Bind
	PlayerInputComponent->BindAction("Jump",EInputEvent::IE_Pressed,this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryAction",EInputEvent::IE_Pressed,this, &ACPlayer::PrimaryAction);
	PlayerInputComponent->BindAction("PrimaryInteraction",EInputEvent::IE_Pressed,this, &ACPlayer::PrimaryInteraction);
	PlayerInputComponent->BindAction("SecondaryAction",EInputEvent::IE_Pressed,this, &ACPlayer::SecondaryAction);
	PlayerInputComponent->BindAction("ThirdAction",EInputEvent::IE_Pressed,this, &ACPlayer::ThirdAction);
	PlayerInputComponent->BindAction("Sprint",EInputEvent::IE_Pressed,this, &ACPlayer::StartSprint);
	PlayerInputComponent->BindAction("Sprint",EInputEvent::IE_Released,this, &ACPlayer::StopSprint);

}

FVector ACPlayer::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

void ACPlayer::OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		float RageDelta = FMath::Abs(Delta);
		AttributeComp->ApplyRageChange(this, RageDelta);
	}

	if (NewHealth <= 0.f && Delta < 0.f)
	{
		APlayerController* PC = GetController<APlayerController>();
		DisableInput(PC);
	}
}

void ACPlayer::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = ControlRot.Roll = 0.f;

	AddMovementInput(ControlRot.Vector(),Value);
}

void ACPlayer::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = ControlRot.Roll = 0.f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void ACPlayer::PrimaryAction()
{
	ActionComp->StartActionByName(this, "MagicBall");
}


void ACPlayer::SecondaryAction()
{
	ActionComp->StartActionByName(this, "WarpBall");
}


void ACPlayer::ThirdAction()
{
	ActionComp->StartActionByName(this, "BlackHole");
}



void ACPlayer::PrimaryInteraction()
{
	if (ensure(InteractionComp))
	{
		InteractionComp->PrimaryInteraction();
	}
}

void ACPlayer::StartSprint()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ACPlayer::StopSprint()
{
	ActionComp->StopActionByName(this, "Sprint");
}
