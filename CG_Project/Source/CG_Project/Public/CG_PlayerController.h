#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CG_PlayerController.generated.h"

// 미리 선언(전방선언?)
class UInputMappingContext;
class UInputAction;

UCLASS()
class CG_PROJECT_API ACG_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// 생성자
	ACG_PlayerController();

	// IMC를 넣어줄 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;

	// InputAction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* HUDWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	UUserWidget* MainMenuWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> EndMenuWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	UUserWidget* EndMenuWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	TSubclassOf<UUserWidget> LevelUpWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	UUserWidget* LevelUpWidgetInstance;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	UUserWidget* GetHUDWidget() const;
	//HUD 표시
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameHUD();
	//게임시작 메뉴 표시
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowGameStartMenu();
	//게임오버 메뉴 표시
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowGameOverMenu(bool bIsClear);
	//게임 시작
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void StartGame();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ExitGame();


	virtual void BeginPlay() override;
};

