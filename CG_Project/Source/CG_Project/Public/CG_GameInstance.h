#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CG_GameInstance.generated.h"

UCLASS()
class CG_PROJECT_API UCG_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UCG_GameInstance();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore; //총점
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex; //현재 레벨

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount); //점수 추가

};
