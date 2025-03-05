#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "CG_GameState.h"

ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//�ڽ� ������Ʈ ����, �� ������ ��Ʈ�� ����
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
    //1. �ڽ� ������Ʈ�� ������ �� Extent �������� �������� ����
    FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
    //2. �ڽ� �߽� ��ġ
    FVector BoxOrigin = SpawningBox->GetComponentLocation();
    //3. �� �ະ�� ������ �� ����
    return BoxOrigin + FVector(
        FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
        FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
        0.0f // Z�� ���� ����
    );
}

//AActor* ASpawnVolume::SpawnEnemy()
//{
//    FVector EnemyLocation = GetRandomPointInVolume();
//    EnemyLocation.Z = 0;
//    AActor* SpawnedActor = GetWorld()->SpawnActor<AEnemy>(
//        DefaultEnemyClass,
//        EnemyLocation,
//        FRotator::ZeroRotator
//    );
//
//    return SpawnedActor;
//}

