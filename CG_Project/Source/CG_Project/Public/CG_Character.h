#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WeaponInterface.h"
#include "StickyGun.h"
#include "HAL/CriticalSection.h"
#include "CG_Character.generated.h"

// 3인칭 카메라 조절용
class USpringArmComponent;

// 3인칭 카메라
class UCameraComponent;

// OverheadWidget을 위한 전방선언
class UWidgetComponent;

// 이동 관련
struct FInputActionValue;

//끈끈이 총
class AStickyGun;

UCLASS()
class CG_PROJECT_API ACG_Character : public ACharacter
{
	GENERATED_BODY()

public:
	ACG_Character();

	// 3인칭 카메라 조절용
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	// 3인칭 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	// OverheadWidget 관련
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget;

	// 체력 관련 get함수, 체력 회복
	UFUNCTION(BlueprintPure, Category = "health")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "health")
	void HealHealth(float Amount);

protected:
	// 최대 체력, 현재 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHealth;

	// 최대 경험치, 현재 경험치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exp")
	float MaxExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exp")
	float CurrentExp;

	// 현재 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	float CurrentLevel;

	virtual void BeginPlay() override;

	// 플레이어 입력 관련
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//발사 액션
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* FireAction;

	//// 플레이어 받은 데미지
	//virtual float TakeDamage();

	//// 플레이어 주는 데미지 *레벨 비례 공식이 들어가야함
	//virtual float DealDamage();

	// 플레이어 이동 관련
	// 참조(FInputActionValue&)로 가져오는 이유는 안그러면 불러오는데 리소스가 너무 많이 들어가서
	// 리플렉션에 등록을 해놔야 엔진에서 얘내들을 인식 할 수 있음
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
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();
	UFUNCTION()
	void Roll(const FInputActionValue& value);
	//무기 획득 함수
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon();

	// 플레이어 사망
	void OnDeath();

	// 머리 위 체력바 관련
	void UpdateOverheadHP();

private:
	// 기본 이동속도
	float NormalSpeed;

	// 달리기 속도 배율
	float SprintSpeedMultiplier;

	// 달리기 속도
	float SprintSpeed;
	//장착된 무기
	//AStickyGun* EquippedWeapon;
	TScriptInterface<IWeaponInterface> EquippedWeapon;
	
	// StickyGun 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AStickyGun> StickyGunClass;

	// 이동, 사격 동시 접근 제한
	FCriticalSection MoveInputSection;
	FVector2D MoveInputValue;

	FCriticalSection FireTimeSection;
	
};