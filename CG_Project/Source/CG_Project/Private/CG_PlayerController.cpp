#include "CG_PlayerController.h"
#include "CG_GameState.h"
#include "CG_GameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

// ������, Ȥ�� �𸣴� �����ϰ� �ϱ� ���� nullptr ����
ACG_PlayerController::ACG_PlayerController() 
	:
	InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr)
{
}

void ACG_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// �����÷��̾� ���� ��������
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		// EnhancedInputSystem�� �����ϴ� Sybsystem ȹ���ϱ�
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			// Subsystem�� IMC�߰��ϰ�(�켱���� �ֿ켱����)
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}