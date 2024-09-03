#include "CAction_ProjectileAttack.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UCAction_ProjectileAttack::UCAction_ProjectileAttack()
{
	AttackDelay = 0.2f;
	HandSocketName = "Muzzle_01";
}

void UCAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{

		if (AttackMontage)
		{
			Character->PlayAnimMontage(AttackMontage);
		}
		if (MuzzleParticle)
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleParticle, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		}

		if (Character->HasAuthority())
		{
		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDealy_Elapsed", Character);
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay,Delegate, AttackDelay,false);
		}
	}
	
}

void UCAction_ProjectileAttack::AttackDealy_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensure(ProjectileClass))
	{
		FVector HandLoc = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		//Trace
		FCollisionObjectQueryParams ObjectQueries;
		ObjectQueries.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueries.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueries.AddObjectTypesToQuery(ECC_Pawn);

		FCollisionShape Shape;
		Shape.SetSphere(20.f);

		FCollisionQueryParams QueryParm;
		QueryParm.AddIgnoredActor(InstigatorCharacter);

		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 5000.f);
		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjectQueries, Shape, QueryParm))
		{
			TraceEnd = Hit.ImpactPoint;
		}

		FRotator ProjectileRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLoc).Rotator();

		//Spawn projectile
		FTransform SpawnTM(ProjectileRotation, HandLoc);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}
	StopAction(InstigatorCharacter);
}