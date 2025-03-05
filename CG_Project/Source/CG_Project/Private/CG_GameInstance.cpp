#include "CG_GameInstance.h"

UCG_GameInstance::UCG_GameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void UCG_GameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Log, TEXT("Total Score Updated: %d"), TotalScore);
}