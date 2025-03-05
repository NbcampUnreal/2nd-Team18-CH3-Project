#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"


AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComp);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComp);

	DamageAmount = 20.0f;
	WeaponRange = 10000.0f;
}

void AWeapon::Fire()
{
	FVector StartLocation = GetOwner()->GetActorLocation();
	FVector ForwardVector = GetOwner()->GetActorForwardVector();
	FVector EndLocation = StartLocation + (ForwardVector * WeaponRange);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.bTraceComplex = true;

	FHitResult HitResult;
	bool bIsHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);
	if (bIsHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			/*FDamageEvent DamageEvent;
			HitActor->TakeDamage(DamageAmount, DamageEvent, nullptr, this);
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s, Applied Damage: %.1f"), *HitActor->GetName(), DamageAmount);*/
		}

		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 16, FColor::Green, false, 2.0f);
		DrawDebugLine(GetWorld(), StartLocation, HitResult.ImpactPoint, FColor::Green, false, 2.0f, 0, 1.0f);
	}
	else
	{
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 2.0f, 0, 1.0f);
	}
}


