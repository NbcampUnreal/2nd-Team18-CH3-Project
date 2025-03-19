// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class CG_PROJECT_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;
	//스폰 영역을 담당할 박스 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;

	//스폰 볼륨 내부 무작위 좌표를 얻어오는 함수
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	FVector GetRandomPointInVolume() const;

	//적을 스폰하는 함수
	//UFUNCTION(BlueprintCallable, Category = "Spawning")
	//AActor* SpawnEnemy();

	//소환할 몬스터 목록
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TArray<TSubclassOf<AActor>> EnemyClasses;
};
