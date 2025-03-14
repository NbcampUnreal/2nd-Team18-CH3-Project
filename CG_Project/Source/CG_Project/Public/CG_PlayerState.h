#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CG_PlayerState.generated.h"

UCLASS()
class CG_PROJECT_API ACG_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACG_PlayerState();

	//����ġ ȹ�� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Level")
	void GainExp(int32 Exp);
	//������ �Լ�
	UFUNCTION(BlueprintCallable, Category = "Level")
	void LevelUp();

	//���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
	int32 PlayerLevel;
	//���� ����ġ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
	int32 CurrentExp;
	//���� ���� �ʿ� ����ġ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
	int32 ExpToLevelUp;
};
