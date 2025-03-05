#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CG_GameState.generated.h"

UCLASS()
class CG_PROJECT_API ACG_GameState : public AGameState
{
	GENERATED_BODY()
	

public:
	ACG_GameState();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	int32 SpawnedEnemyCount; //������ų �� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	int32 DestroyEnemyCount; //���� �� ��

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 CurrentLevelIndex; //���� ���̺�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 MaxLevels; //�ִ� ���̺�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	float LevelDuration; //���̺� ���� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<int32> EnemyToSpawnPerWave; //���̺� �� �� ����

	FTimerHandle WaveTimerHandle; //���̺� Ÿ�̸��ڵ�
	FTimerHandle HUDUpdateTimerHandle; //HUD ������Ʈ

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score; //���� ���� ����
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const; //���� ����
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount); //���� �߰� �Լ�

	//���ӿ���
	void OnGameOver();
	//���� �����߷��� �� ȣ��
	void OnEnemyDestroyed();
	//�� ����
	//void GenerateEnemy();

	//���̺� ����, ��
	void StartWave();
	void EndWave();
	//���̺� �ð� ����
	void OnWaveTimeUp();
	//HUD ������Ʈ
	void UpdateHUD();
};
