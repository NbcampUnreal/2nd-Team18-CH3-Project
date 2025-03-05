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

	//HUD ������Ʈ
	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ACG_GameState::UpdateHUD,
		0.1f,
		true
	);
}

//���̺� ����
void ACG_GameState::StartWave()
{
	UpdateHUD();
}

//���̺� ����
void ACG_GameState::EndWave()
{

}

//���̺� �ð� ����
void ACG_GameState::OnWaveTimeUp()
{
	//�ð� ���� �� ���� ��ȯ
}

//���� ����
int32 ACG_GameState::GetScore() const
{
	return Score;
}

//���� �߰�
void ACG_GameState::AddScore(int32 Amount)
{

}


//���� �����߷��� �� ȣ��
void ACG_GameState::OnEnemyDestroyed()
{
	DestroyEnemyCount++;
	UE_LOG(LogTemp, Warning, TEXT("Enemy Destroyed: %d / %d"), SpawnedEnemyCount, DestroyEnemyCount);
	//���� ���� ������ �� ���� �������� �� ���� �Լ� ȣ��
	if (SpawnedEnemyCount > 0 && DestroyEnemyCount >= SpawnedEnemyCount)
	{
		/*GenerateEnemy();*/
	}
}

//�� ����
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

//���ӿ���
void ACG_GameState::OnGameOver()
{
	UpdateHUD();
	UE_LOG(LogTemp, Warning, TEXT("Game Over!!"));
}

//HUD ������Ʈ
void ACG_GameState::UpdateHUD()
{

}