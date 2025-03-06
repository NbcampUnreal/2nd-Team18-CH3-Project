#include "CG_GameMode.h"
#include "CG_GameState.h"
#include "CG_Character.h"
#include "CG_PlayerController.h"

ACG_GameMode::ACG_GameMode()
{
	//GameState�� ������Ʈ���� ���� ACG_GameState�� ����
	GameStateClass = ACG_GameState::StaticClass();

	//CG_PlayerController�� ���� ��Ʈ�ѷ��� ����
	PlayerControllerClass = ACG_PlayerController::StaticClass();

	//CG_Character�� ���� ĳ���ͷ� ����
	DefaultPawnClass = ACG_Character::StaticClass();
}