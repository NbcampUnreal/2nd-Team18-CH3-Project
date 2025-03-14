#include "CG_PlayerState.h"

ACG_PlayerState::ACG_PlayerState()
{
	PlayerLevel = 1;
	CurrentExp = 0;
	ExpToLevelUp = 10;
}

void ACG_PlayerState::GainExp(int32 Exp)
{
	CurrentExp += Exp;

	//����ġ �ʿ�� ���� �� ������
	if (CurrentExp >= ExpToLevelUp)
	{
		LevelUp();
	}
}

void ACG_PlayerState::LevelUp()
{
	PlayerLevel++;
	CurrentExp = 0;
	ExpToLevelUp += 10;

}

