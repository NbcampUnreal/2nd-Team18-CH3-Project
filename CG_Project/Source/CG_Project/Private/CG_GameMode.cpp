#include "CG_GameMode.h"
#include "CG_GameState.h"

ACG_GameMode::ACG_GameMode()
{
	GameStateClass = ACG_GameState::StaticClass();
}