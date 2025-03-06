#include "CG_GameMode.h"
#include "CG_GameState.h"
#include "CG_Character.h"
#include "CG_PlayerController.h"

ACG_GameMode::ACG_GameMode()
{
	//GameState를 프로젝트에서 만든 ACG_GameState로 설정
	GameStateClass = ACG_GameState::StaticClass();

	//CG_PlayerController를 메인 컨트롤러로 변경
	PlayerControllerClass = ACG_PlayerController::StaticClass();

	//CG_Character를 메인 캐릭터로 설정
	DefaultPawnClass = ACG_Character::StaticClass();
}