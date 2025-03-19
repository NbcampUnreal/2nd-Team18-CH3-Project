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
	SprintAction(nullptr),
	FireAction(nullptr),
	RollAction(nullptr),
	HUDWidgetInstance(nullptr),
	MainMenuWidgetClass(nullptr),
	MainMenuWidgetInstance(nullptr)
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

	//게임 실행 시 메뉴레벨에서 메뉴 UI 먼저 표시
	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		ShowGameStartMenu();
	}
}

UUserWidget* ACG_PlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

//메뉴 UI 표시
void ACG_PlayerController::ShowGameStartMenu()
{	//HUD가 켜져 있다면 닫기
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
	//이미 메뉴가 떠있으면 제거
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}
	//종료 메뉴가 떠있으면 제거
	if (EndMenuWidgetInstance)
	{
		EndMenuWidgetInstance->RemoveFromParent();
		EndMenuWidgetInstance = nullptr;
	}
	//레벨업 메뉴가 떠있으면 제거
	if (LevelUpWidgetInstance)
	{
		LevelUpWidgetInstance->RemoveFromParent();
		LevelUpWidgetInstance = nullptr;
	}

	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}

void ACG_PlayerController::ShowGameOverMenu(bool bIsClear)
{	//HUD가 켜져 있다면 닫기
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
	//이미 메뉴가 떠있으면 제거
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}
	//종료 메뉴가 떠있으면 제거
	if (EndMenuWidgetInstance)
	{
		EndMenuWidgetInstance->RemoveFromParent();
		EndMenuWidgetInstance = nullptr;
	}
	//레벨업 메뉴가 떠있으면 제거
	if (LevelUpWidgetInstance)
	{
		LevelUpWidgetInstance->RemoveFromParent();
		LevelUpWidgetInstance = nullptr;
	}

	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}

		if (UTextBlock *Text = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("clearNover"))))
		{
			if (bIsClear)
			{
				Text->SetText(FText::FromString(TEXT("Game Clear!")));
			}

			else
			{
				Text->SetText(FText::FromString(TEXT("GameOver")));
			}
		}
	}
}

//게임 HUD 표시
void ACG_PlayerController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{	//HUD가 켜져있다면 닫기
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{	//이미 메뉴가 떠있으면 제거
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	//종료 메뉴가 떠있으면 제거
	if (EndMenuWidgetInstance)
	{
		EndMenuWidgetInstance->RemoveFromParent();
		EndMenuWidgetInstance = nullptr;
	}

	//레벨업 메뉴가 떠있으면 제거
	if (LevelUpWidgetInstance)
	{
		LevelUpWidgetInstance->RemoveFromParent();
		LevelUpWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());

			ACG_GameState* CG_GameState = GetWorld() ? GetWorld()->GetGameState<ACG_GameState>() : nullptr;
			if (CG_GameState)
			{
				CG_GameState->UpdateHUD();
			}
		}
	}
}
//게임 시작 - 게임 레벨 오픈, 게임인스턴스 데이터 리셋
void ACG_PlayerController::StartGame()
{
	if (UCG_GameInstance* CG_GameInstance = Cast<UCG_GameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		CG_GameInstance->CurrentLevelIndex = 0;
		CG_GameInstance->TotalScore = 0;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("StageLevel"));
}

void ACG_PlayerController::ExitGame()
{
	//게임 완전 종료
	UKismetSystemLibrary::QuitGame(
		GetWorld(),
		this,
		EQuitPreference::Quit,
		false
	);
}
