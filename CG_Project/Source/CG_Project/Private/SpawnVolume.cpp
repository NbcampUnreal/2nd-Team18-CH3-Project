#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "CG_GameState.h"

ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//박스 컴포넌트 생성, 이 액터의 루트로 설정
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
    //1. 박스 컴포넌트의 스케일 된 Extent 방향으로 반지름을 구함
    FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
    //2. 박스 중심 위치
    FVector BoxOrigin = SpawningBox->GetComponentLocation();
    //3. 각 축별로 무작위 값 생성
    return BoxOrigin + FVector(
        FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
        FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
        0.0f // Z축 값을 고정
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

