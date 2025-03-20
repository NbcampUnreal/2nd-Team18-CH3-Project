// Fill out your copyright notice in the Description page of Project Settings.


#include "CG_Target.h"
#include "TargetSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "CG_GameState.h"
#include "Components/SphereComponent.h"

// Sets default values
ACG_Target::ACG_Target()
{
	PrimaryActorTick.bCanEverTick = false;

    // 루트 컴포넌트 생성 및 설정
    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    // 충돌 컴포넌트 생성 및 설정
    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    // 겹침만 감지하는 프로파일 설정
    Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    // 루트 컴포넌트로 설정
    Collision->SetupAttachment(Scene);

    // 스태틱 메시 컴포넌트 생성 및 설정
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(Collision);
}

void ACG_Target::BeginPlay()
{
    Super::BeginPlay();

    // Overlap 이벤트 바인딩
    Collision->OnComponentBeginOverlap.AddDynamic(this, &ACG_Target::OnItemOverlap);
}

void ACG_Target::OnItemOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    // OtherActor가 플레이어인지 확인 ("Player" 태그 활용)
    if (OtherActor && OtherActor->ActorHasTag("Bullet"))
    {
        ATargetSpawner* TargetSpawner = Cast<ATargetSpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), ATargetSpawner::StaticClass()));

        if (TargetSpawner)
        {
            TargetSpawner->OnTargetHit();
            ACG_GameState* CG_GameState = Cast<ACG_GameState>(UGameplayStatics::GetActorOfClass(GetWorld(), ACG_GameState::StaticClass()));
            if (CG_GameState) CG_GameState->AddScore(10);
        }

        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Miss!")));
        }
    }
}