#include "CG_PlayerController.h"
#include "CG_GameState.h"
#include "CG_GameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

// 생성자, 혹시 모르니 안전하게 하기 위해 nullptr 기입
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

	// 로컬플레이어 정보 가져오기
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		// EnhancedInputSystem을 관리하는 Sybsystem 획득하기
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			// Subsystem에 IMC추가하고(우선순위 최우선으로)
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}