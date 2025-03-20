#include "TargetSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"


void ATargetSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnTarget();
	TargetSpawnTime = 3.0f;
}

void ATargetSpawner::SpawnTarget()
{
	TargetSpawnTime = 3.0f;

	if (!TargetClass) return;

	//랜덤 위치 생성
	FVector SpawnLocation = GetRandomSpawnLocation();
	
	//플레이어 위치 가져오기
	AActor* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Player) return;

	FVector PlayerLocation = Player->GetActorLocation();

	//플레이어를 바라보는 회전값 계산
	FRotator LookAtRotation = (PlayerLocation - SpawnLocation).Rotation();

	//과녁 스폰
	CurrentTarget = GetWorld()->SpawnActor<AActor>(TargetClass, SpawnLocation, LookAtRotation);

	//스폰 사운드 재생
	if (SpawnSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, SpawnSound, SpawnLocation);
	}
}

FVector ATargetSpawner::GetRandomSpawnLocation()
{
	float X = FMath::RandRange(MinX, MaxX);
	float Y = FMath::RandRange(MinY, MaxY);
	float Z = SpawnHeight;
	return FVector(X, Y, Z);
}

void ATargetSpawner::OnTargetHit()
{
	if (CurrentTarget)
	{
		CurrentTarget->Destroy();
		CurrentTarget = nullptr;

		//타격 사운드 재생
		if (HitSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(this, HitSound, GetActorLocation());
		}

		//3초 뒤 새 과녁 생성
		GetWorld()->GetTimerManager().SetTimer(
			RespawnTimer,
			this,
			&ATargetSpawner::SpawnTarget,
			TargetSpawnTime,
			false
		);
	}
}