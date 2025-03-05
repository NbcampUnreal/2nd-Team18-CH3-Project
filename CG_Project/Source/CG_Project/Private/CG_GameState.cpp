#include "CG_GameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "CG_PlayerController.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

ACG_GameState::ACG_GameState()
{
	SpawnedEnemyCount = 0;
	DestroyEnemyCount = 0;
	CurrentLevelIndex = 0;
	MaxLevels = 0;
	LevelDuration = 0;
	EnemyToSpawnPerWave = {10};
	Score = 0;
	

}

void ACG_GameState::BeginPlay()
{
	Super::BeginPlay();

	UpdateHUD();
	StartWave();

	//HUD 업데이트
	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ACG_GameState::UpdateHUD,
		0.1f,
		true
	);
}

//웨이브 시작
void ACG_GameState::StartWave()
{
	UpdateHUD();
}

//웨이브 종료
void ACG_GameState::EndWave()
{

}

//웨이브 시간 종료
void ACG_GameState::OnWaveTimeUp()
{
	//시간 동결 후 보스 소환
}

//현재 점수
int32 ACG_GameState::GetScore() const
{
	return Score;
}

//점수 추가
void ACG_GameState::AddScore(int32 Amount)
{

}


//적을 쓰러뜨렸을 때 호출
void ACG_GameState::OnEnemyDestroyed()
{
	DestroyEnemyCount++;
	UE_LOG(LogTemp, Warning, TEXT("Enemy Destroyed: %d / %d"), SpawnedEnemyCount, DestroyEnemyCount);
	//잡은 적이 스폰된 적 수와 같아지면 적 생성 함수 호출
	if (SpawnedEnemyCount > 0 && DestroyEnemyCount >= SpawnedEnemyCount)
	{
		/*GenerateEnemy();*/
	}
}

//적 생성
//void ACG_GameState::GenerateEnemy()
//{
//	TArray<AActor*> FoundVolumes;
//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
//
//	for (int32 i = 0; i < SpawnedEnemyCount; i++)
//	{
//		if (FoundVolumes.Num() > 0)
//		{
//			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
//			if (SpawnVolume)
//			{
//				AActor* SpanwedActor = SpawnVolume->SpawnEnemy();
//			}
//		}
//	}
//}

//게임오버
void ACG_GameState::OnGameOver()
{
	UpdateHUD();
	UE_LOG(LogTemp, Warning, TEXT("Game Over!!"));
}

//HUD 업데이트
void ACG_GameState::UpdateHUD()
{

}