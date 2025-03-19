#include "StickyGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "StickyBullet.h"
#include "Kismet/GameplayStatics.h"

AStickyGun::AStickyGun()
{
	// 루트 컴포넌트 설정
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	// 무기 메시 생성 및 설정
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	// 발사 위칭 생성 및 설정
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(WeaponMesh);

	FireDelay = 0.5f;
	Damage = 70.0f;
}


void AStickyGun::BeginPlay()
{
	Super::BeginPlay();

	LastFireTime = 0.0f;
}


void AStickyGun::Fire()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	if (CurrentTime > LastFireTime + FireDelay)
	{
		if (ProjectileClass)
		{
			UWorld* World = GetWorld();

			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetInstigator();

				FVector SpawnLocation = MuzzleLocation->GetComponentLocation();
				FRotator SpawnRotation = MuzzleLocation->GetComponentRotation();

				//DrawDebugBox(this, SpawnLocation);
				
				AStickyBullet* Bullet = World->SpawnActor<AStickyBullet>(
					ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

				if (Bullet != nullptr)
				{
					
					//발사체 데미지 
					Bullet->SetDamage(Damage);
					// 발사 시간 갱신
					LastFireTime = CurrentTime;
					// 사운드
					UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
					// 애니메이션 재생
					UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance();
					
					if (AnimInstance && FireAnimation)
					{
						AnimInstance->Montage_Play(FireAnimation, 1.0f);
					}
				}
			}
		}
	}
}
float AStickyGun::GetDamage()
{
	return Damage;
}
