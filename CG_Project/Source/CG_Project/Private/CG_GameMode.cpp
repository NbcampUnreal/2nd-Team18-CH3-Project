#include "CG_GameMode.h"
#include "CG_GameState.h"

ACG_GameMode::ACG_GameMode()
{
	//GameState�� ������Ʈ���� ���� ACG_GameState�� ����
	GameStateClass = ACG_GameState::StaticClass();
}