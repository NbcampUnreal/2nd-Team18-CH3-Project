#include "StickyBullet.h"
#include "Monster.h"
#include "Kismet/GameplayStatics.h"

AStickyBullet::AStickyBullet()
{
 
	//메시 컴포넌트 생성 및 설정
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	RootComponent = BulletMesh;

	//투사체 이동 컴포넌트 생성 및 설정
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	//충돌 이벤트 활성화
	BulletMesh->SetNotifyRigidBodyCollision(true);

	
	Damage = 10.0f;

}


void AStickyBullet::BeginPlay()
{
	Super::BeginPlay();

	// 충돌 이벤트 핸들러 바인딩
	BulletMesh->OnComponentHit.AddDynamic(this, &AStickyBullet::OnHit);
}

void AStickyBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						  FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		// 맞은놈이 몬스터냐?
		AMonster* Monster = Cast<AMonster>(OtherActor);
		
		if (Monster)
		{
			// 데미지 줘라
			UGameplayStatics::ApplyPointDamage
			(
				OtherActor, Damage, NormalImpulse, Hit,
				GetInstigatorController(),
				this,
				UDamageType::StaticClass()
			);
			// 느려지게
			//Monster->SlowDown(3.0f);
			//피격 사운드 재생
			UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
		
			Destroy();
		}
		else
		{
			{
				Destroy();
			}
		}
	}
}
