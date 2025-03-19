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

	//���� ���� �� �޴��������� �޴� UI ���� ǥ��
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

//�޴� UI ǥ��
void ACG_PlayerController::ShowGameStartMenu()
{	//HUD�� ���� �ִٸ� �ݱ�
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
	//�̹� �޴��� �������� ����
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}
	//���� �޴��� �������� ����
	if (EndMenuWidgetInstance)
	{
		EndMenuWidgetInstance->RemoveFromParent();
		EndMenuWidgetInstance = nullptr;
	}
	//������ �޴��� �������� ����
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
{	//HUD�� ���� �ִٸ� �ݱ�
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
	//�̹� �޴��� �������� ����
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}
	//���� �޴��� �������� ����
	if (EndMenuWidgetInstance)
	{
		EndMenuWidgetInstance->RemoveFromParent();
		EndMenuWidgetInstance = nullptr;
	}
	//������ �޴��� �������� ����
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

//���� HUD ǥ��
void ACG_PlayerController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{	//HUD�� �����ִٸ� �ݱ�
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{	//�̹� �޴��� �������� ����
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	//���� �޴��� �������� ����
	if (EndMenuWidgetInstance)
	{
		EndMenuWidgetInstance->RemoveFromParent();
		EndMenuWidgetInstance = nullptr;
	}

	//������ �޴��� �������� ����
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
//���� ���� - ���� ���� ����, �����ν��Ͻ� ������ ����
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
	//���� ���� ����
	UKismetSystemLibrary::QuitGame(
		GetWorld(),
		this,
		EQuitPreference::Quit,
		false
	);
}
