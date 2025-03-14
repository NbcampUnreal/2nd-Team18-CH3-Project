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

	//경험치 획득 함수
	UFUNCTION(BlueprintCallable, Category = "Level")
	void GainExp(int32 Exp);
	//레벨업 함수
	UFUNCTION(BlueprintCallable, Category = "Level")
	void LevelUp();

	//현재 레벨
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
	int32 PlayerLevel;
	//현재 경험치
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
	int32 CurrentExp;
	//다음 레벨 필요 경험치
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player State")
	int32 ExpToLevelUp;
};
