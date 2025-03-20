// Fill out your copyright notice in the Description page of Project Settings.


#include "CG_Target.h"

// Sets default values
ACG_Target::ACG_Target()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACG_Target::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACG_Target::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

