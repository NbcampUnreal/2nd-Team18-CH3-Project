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
	int32 TotalScore; //����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex; //���� ����

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount); //���� �߰�

};
