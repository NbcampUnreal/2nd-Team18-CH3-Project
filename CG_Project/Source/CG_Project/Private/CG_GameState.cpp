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
	LevelDuration = 60.0f;
	Min = 10;
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
	if (UCG_GameInstance* CG_GameInstance = GetCG_GameInstance())
	{
		CurrentLevelIndex = CG_GameInstance->CurrentLevelIndex;
	}
	//카운트 초기화, 이전 Wave 몹 제거
	SpawnedEnemyCount = 0;
	DestroyEnemyCount = 0;

	//Wave 타이머 시작
	GetWorldTimerManager().SetTimer(
		WaveTimerHandle,
		this,
		&ACG_GameState::OnWaveTimeUp,
		LevelDuration,
		true
	);
	UpdateHUD();
}

//웨이브 종료
void ACG_GameState::EndWave()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle); //WaveTimerHandle 초기화

	++CurrentLevelIndex; //레벨 증가
	if (CurrentLevelIndex >= MaxLevels)
	{
		EndGame();
	}
	else
	{
		StartWave();
	}
}

//웨이브 시간 종료
void ACG_GameState::OnWaveTimeUp()
{
	if (Min > 0) //분이 0이 되면
	{
		--Min;
	}
	else
	{
		GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	}


	//시간 동결 후 보스 소환
}

void ACG_GameState::EndGame()
{

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
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ACG_PlayerController* CG_PlayerController = Cast<ACG_PlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = CG_PlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimerText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Timer"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle);
					if (GetWorldTimerManager().IsTimerActive(WaveTimerHandle))
					{
						TimerText->SetText(FText::FromString(FString::Printf(TEXT("%d : %.0f"), Min, RemainingTime)));
					}
					else
					{
						TimerText->SetText(FText::FromString(FString::Printf(TEXT("Boss Stage"))));
					}
				}
			}
		}
	}
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