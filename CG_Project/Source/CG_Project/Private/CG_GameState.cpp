#include "CG_GameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "CG_PlayerController.h"
#include "CG_GameInstance.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

ACG_GameState::ACG_GameState()
{
	SpawnedEnemyCount = 0;
	DestroyEnemyCount = 0;
	CurrentLevelIndex = 0;
	MaxLevels = 0;
	LevelDuration = 0.0f;
	Min = 0;
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
	if (UCG_GameInstance* CG_GameInstance = GetCG_GameInstance())
	{
		CurrentLevelIndex = CG_GameInstance->CurrentLevelIndex;
	}
	//ī��Ʈ �ʱ�ȭ, ���� Wave �� ����
	SpawnedEnemyCount = 0;
	DestroyEnemyCount = 0;

	//Wave Ÿ�̸� ����
	Min = 10;
	LevelDuration = 60.0f;
	GetWorldTimerManager().SetTimer(
		WaveTimerHandle,
		this,
		&ACG_GameState::OnWaveTimeUp,
		LevelDuration,
		true
	);
	UpdateHUD();
}

//���̺� ����
void ACG_GameState::EndWave()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle); //WaveTimerHandle �ʱ�ȭ

	++CurrentLevelIndex; //���� ����
	if (CurrentLevelIndex >= MaxLevels)
	{
		EndGame();
	}
	else
	{
		StartWave();
	}
}

//���̺� �ð� ����
void ACG_GameState::OnWaveTimeUp()
{
	--Min;
	if (Min <= 0) //���� 0�� �Ǹ�
	{
		//WaveTimerHandle�� �۵������� Ȯ���ϰ�
		if (!GetWorldTimerManager().IsTimerActive(WaveTimerHandle)) 
		{	//�۵����̸� �ݺ��� ����
			GetWorldTimerManager().SetTimer(
				WaveTimerHandle,
				this,
				&ACG_GameState::OnWaveTimeUp,
				LevelDuration,
				false
			);
		}
	}

	//�ð� ���� �� ���� ��ȯ
}

void ACG_GameState::EndGame()
{

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

//ASpawnVolume* ACG_GameState::GetSpawnVolume() const
//{
//	return
//}

ACG_PlayerController* ACG_GameState::GetCG_PlayerController() const
{
	return Cast<ACG_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

UCG_GameInstance* ACG_GameState::GetCG_GameInstance() const
{
	return Cast<UCG_GameInstance>(GetGameInstance());
}