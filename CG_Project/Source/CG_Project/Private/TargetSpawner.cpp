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

	//���� ��ġ ����
	FVector SpawnLocation = GetRandomSpawnLocation();
	
	//�÷��̾� ��ġ ��������
	AActor* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Player) return;

	FVector PlayerLocation = Player->GetActorLocation();

	//�÷��̾ �ٶ󺸴� ȸ���� ���
	FRotator LookAtRotation = (PlayerLocation - SpawnLocation).Rotation();

	//���� ����
	CurrentTarget = GetWorld()->SpawnActor<AActor>(TargetClass, SpawnLocation, LookAtRotation);

	//���� ���� ���
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

		//Ÿ�� ���� ���
		if (HitSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(this, HitSound, GetActorLocation());
		}

		//3�� �� �� ���� ����
		GetWorld()->GetTimerManager().SetTimer(
			RespawnTimer,
			this,
			&ATargetSpawner::SpawnTarget,
			TargetSpawnTime,
			false
		);
	}
}