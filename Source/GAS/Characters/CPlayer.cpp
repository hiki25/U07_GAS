#include "CPlayer.h"
#include "Engine.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CAttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CInteractionComponent.h"

ACPlayer::ACPlayer()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");

	InteractionComp = CreateDefaultSubobject<UCInteractionComponent>("InteractionComp");
	AttributeComp = CreateDefaultSubobject<UCAttributeComponent>("AttributeComp");

	//Component Attach
	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp->SetupAttachment(SpringArmComp);

	//SpringArmComp Setting
	SpringArmComp->bUsePawnControlRotation = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	AttackDelay = 0.2f;
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

}

void ACPlayer::OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewHealth, float Delta)
{
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
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAction,this, &ACPlayer::PrimaryAction_TimeElapsed, 0.2f);

	
}

void ACPlayer::PrimaryAction_TimeElapsed()
{
	SpawnProjectile(MagicBallClass);
}

void ACPlayer::SecondaryAction()
{
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}

	GetWorldTimerManager().SetTimer(TimerHandle_SecondartAction, this, &ACPlayer::SecondaryAction_TimeElapsed, 0.2f);
}

void ACPlayer::SecondaryAction_TimeElapsed()
{
	SpawnProjectile(WarpBallClass);
}

void ACPlayer::ThirdAction()
{
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}

	GetWorldTimerManager().SetTimer(TimerHandle_ThirdAction, this, &ACPlayer::ThirdAction_TimeElapsed, 0.2f);
}

void ACPlayer::ThirdAction_TimeElapsed()
{
	SpawnProjectile(BlackHoleClass);
}

void ACPlayer::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensure(ClassToSpawn))
	{
		FVector HandLoc = GetMesh()->GetSocketLocation("Muzzle_01");

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		//Trace
		FCollisionObjectQueryParams ObjectQueries;
		ObjectQueries.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueries.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueries.AddObjectTypesToQuery(ECC_Pawn);

		FCollisionShape Shape;
		Shape.SetSphere(20.f);

		FCollisionQueryParams QueryParm;
		QueryParm.AddIgnoredActor(this);

		FVector TraceStart = CameraComp->GetComponentLocation();
		FVector TraceEnd = TraceStart + (GetControlRotation().Vector() * 5000.f);
		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjectQueries, Shape, QueryParm))
		{
			TraceEnd = Hit.ImpactPoint;
		}

		FRotator ProjectileRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLoc).Rotator();

		//Spawn projectile
		FTransform SpawnTM(ProjectileRotation, HandLoc);
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}

}

void ACPlayer::PrimaryInteraction()
{
	if (ensure(InteractionComp))
	{
		InteractionComp->PrimaryInteraction();
	}
}
