#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CG_GameState.generated.h"

class ASpawnVolume;
class ACG_PlayerController;
class UCG_GameInstance;


UCLASS()
class CG_PROJECT_API ACG_GameState : public AGameState
{
	GENERATED_BODY()
	

public:
	ACG_GameState();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	int32 SpawnedEnemyCount; //스폰시킬 적 갯수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	int32 DestroyEnemyCount; //죽인 적 수

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 CurrentLevelIndex; //현재 웨이브
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 MaxLevels; //최대 웨이브
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	float LevelDuration; //웨이브 현재 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 Min; //남은 분
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<int32> EnemyToSpawnPerWave; //웨이브 당 적 스폰

	FTimerHandle WaveTimerHandle; //웨이브 타이머핸들
	FTimerHandle HUDUpdateTimerHandle; //HUD 업데이트 타이머 핸들

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score; //총점 저장 변수
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const; //현재 점수
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount); //점수 추가 함수

	//게임오버
	void OnGameOver();
	//적을 쓰러뜨렸을 때 호출
	void OnEnemyDestroyed();
	//적 생성
	//void GenerateEnemy();

	//웨이브 시작, 끝
	void StartWave();
	void EndWave();
	//웨이브 시간 종료
	void OnWaveTimeUp();
	void EndGame();
	//HUD 업데이트
	void UpdateHUD();

private:
	//헬퍼 함수
	//ASpawnVolume *GetSpawnVolume() const;
	ACG_PlayerController *GetCG_PlayerController() const;
	UCG_GameInstance *GetCG_GameInstance() const;
};
