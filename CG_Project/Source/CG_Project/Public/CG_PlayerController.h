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

	UFUNCTION(BlueprintCallable, Category = "HUD")
	UUserWidget* GetHUDWidget() const;

	virtual void BeginPlay() override;
};

