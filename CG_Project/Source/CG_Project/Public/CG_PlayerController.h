#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CG_PlayerController.generated.h"

// �̸� ����(���漱��?)
class UInputMappingContext;
class UInputAction;

UCLASS()
class CG_PROJECT_API ACG_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// ������
	ACG_PlayerController();

	// IMC�� �־��� ����
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

