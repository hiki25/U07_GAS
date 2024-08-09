#include "CPlayer.h"
#include "Engine.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CInteractionComponent.h"

ACPlayer::ACPlayer()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	InteractionComp = CreateDefaultSubobject<UCInteractionComponent>("InteractionComp");

	//Component Attach
	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp->SetupAttachment(SpringArmComp);

	//SpringArmComp Setting
	SpringArmComp->bUsePawnControlRotation = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	AttackDelay = 0.2f;
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
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
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAction,this, &ACPlayer::PrimaryAction_TimeElapsed, 0.2f);

	
}

void ACPlayer::PrimaryAction_TimeElapsed()
{
	FVector HandLoc = GetMesh()->GetSocketLocation("Muzzle_01");

	FTransform SpawnTM(GetControlRotation(), HandLoc);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	if (ensure(MagicBallClass))
	{
		GetWorld()->SpawnActor<AActor>(MagicBallClass, SpawnTM, SpawnParams);
	}
}

void ACPlayer::PrimaryInteraction()
{
	if (ensure(InteractionComp))
	{
		InteractionComp->PrimaryInteraction();
	}
}
