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

    // ��Ʈ ������Ʈ ���� �� ����
    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    // �浹 ������Ʈ ���� �� ����
    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    // ��ħ�� �����ϴ� �������� ����
    Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    // ��Ʈ ������Ʈ�� ����
    Collision->SetupAttachment(Scene);

    // ����ƽ �޽� ������Ʈ ���� �� ����
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(Collision);
}

void ACG_Target::BeginPlay()
{
    Super::BeginPlay();

    // Overlap �̺�Ʈ ���ε�
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
    // OtherActor�� �÷��̾����� Ȯ�� ("Player" �±� Ȱ��)
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