#include "CG_GameMode.h"
#include "CG_GameState.h"

ACG_GameMode::ACG_GameMode()
{
	//GameState를 프로젝트에서 만든 ACG_GameState로 설정
	GameStateClass = ACG_GameState::StaticClass();
}