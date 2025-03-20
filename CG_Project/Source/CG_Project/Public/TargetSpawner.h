#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetSpawner.generated.h"

UCLASS()
class CG_PROJECT_API ATargetSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	void SpawnTarget();
	void OnTargetHit();

	UPROPERTY(EditAnywhere)
	float TargetSpawnTime;

protected:
	virtual void BeginPlay() override;

private:
	FVector GetRandomSpawnLocation();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> TargetClass; //°ú³á ¿¢ÅÍ

	UPROPERTY(EditAnywhere)
	float MinX = -500.0f, MaxX = 500.0f;

	UPROPERTY(EditAnywhere)
	float MinY = -500.0f, MaxY = 500.0f;

	UPROPERTY(EditAnywhere)
	float SpawnHeight = 100.0f;

	UPROPERTY(EditAnywhere)
	USoundBase* SpawnSound;

	UPROPERTY(EditAnywhere)
	USoundBase* HitSound;

	AActor* CurrentTarget;
	FTimerHandle RespawnTimer;
};
