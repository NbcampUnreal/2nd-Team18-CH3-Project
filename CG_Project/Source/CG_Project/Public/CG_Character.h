#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CG_Character.generated.h"

// 3��Ī ī�޶� ������
class USpringArmComponent;

// 3��Ī ī�޶�
class UCameraComponent;

// OverheadWidget�� ���� ���漱��
class UWidgetComponent;

// �̵� ����
struct FInputActionValue;

UCLASS()
class CG_PROJECT_API ACG_Character : public ACharacter
{
	GENERATED_BODY()

public:
	ACG_Character();

	// 3��Ī ī�޶� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	// 3��Ī ī�޶�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	// OverheadWidget ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget;

	// ü�� ���� get�Լ�, ü�� ȸ��
	UFUNCTION(BlueprintPure, Category = "health")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "health")
	void HealHealth(float Amount);

protected:
	// �ִ� ü��, ���� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHealth;

	// �ִ� ����ġ, ���� ����ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exp")
	float MaxExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exp")
	float CurrentExp;

	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	float CurrentLevel;

	virtual void BeginPlay() override;

	// �÷��̾� �Է� ����
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//// �÷��̾� ���� ������
	//virtual float TakeDamage();

	//// �÷��̾� �ִ� ������ *���� ��� ������ ������
	//virtual float DealDamage();

	// �÷��̾� �̵� ����
	// ����(FInputActionValue&)�� �������� ������ �ȱ׷��� �ҷ����µ� ���ҽ��� �ʹ� ���� ����
	// ���÷��ǿ� ����� �س��� �������� �곻���� �ν� �� �� ����
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);

	// �÷��̾� ���
	void OnDeath();

	// �Ӹ� �� ü�¹� ����
	void UpdateOverheadHP();

private:
	// �⺻ �̵��ӵ�
	float NormalSpeed;

	// �޸��� �ӵ� ����
	float SprintSpeedMultiplier;

	// �޸��� �ӵ�
	float SprintSpeed;
};
