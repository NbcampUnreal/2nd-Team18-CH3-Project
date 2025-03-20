#include "CG_Character.h"
#include "CG_PlayerController.h"
#include "CG_GameState.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "WeaponInterface.h"
#include "StickyGun.h"

ACG_Character::ACG_Character()
{
	PrimaryActorTick.bCanEverTick = false;

	// 카메라 조정 막대 관련
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	// 카메라
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	// 머리 위 HP 관련 위젯
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	// 플레이어 이동속도
	NormalSpeed = 350.0f;
	SprintSpeedMultiplier = 1.8f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	// 캐릭터 체력
	MaxHealth = (CurrentLevel * 10.0f) + 90.0f;
	CurrentHealth = MaxHealth;

	// 캐릭터 레벨 
	CurrentLevel = 1;
}

void ACG_Character::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHP();

	if (StickyGunClass)
	{
		EquipWeapon(); // 무기 소환
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("StickyGunClass is not set!"));
	}
}

// 캐릭터 조작 관련
void ACG_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ACG_PlayerController* PlayerController = Cast<ACG_PlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction
				(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ACG_Character::Move
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction
				(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ACG_Character::StartJump
				);

				EnhancedInput->BindAction
				(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&ACG_Character::StopJump
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction
				(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ACG_Character::Look
				);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction
				(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ACG_Character::StartSprint
				);

				EnhancedInput->BindAction
				(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ACG_Character::StopSprint
				);
			}
			if (PlayerController->RollAction)
			{
				EnhancedInput->BindAction
				(
					PlayerController->RollAction,
					ETriggerEvent::Triggered,
					this,
					&ACG_Character::Roll
				);
			}
			if (PlayerController->FireAction)
			{
				EnhancedInput->BindAction
				(
					PlayerController->FireAction,
					ETriggerEvent::Triggered,
					this,
					&ACG_Character::Fire
				);
			}
		}
	}
}

// 이동
void ACG_Character::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

// 점프(시작)
void ACG_Character::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

// 점프(멈춤)
void ACG_Character::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

// 시점
void ACG_Character::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

// 달리기(시작)
void ACG_Character::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

// 달리기(멈춤)
void ACG_Character::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}


// 구르기 (우클릭)
void ACG_Character::Roll(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Roll Activated!"));
	}
}

// 캐릭터 머리 위 hp바
void ACG_Character::UpdateOverheadHP()
{
	// 추가해야됨
}

// 현재 체력 get함수
float ACG_Character::GetHealth() const
{
	return CurrentHealth;
}

// 체력 회복 매커니즘
void ACG_Character::HealHealth(float Amount)
{
	// Clamp(최소~최대 체력 사이의 값, 최소체력, 최대체력)
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0f, MaxHealth);
	// 계산이 끝난 뒤 체력바 위젯에 업데이트
	UpdateOverheadHP();
}

//// 캐릭터 데미지 공식(레벨 비례 데미지 증가 포함해야 함)
//float ACG_Character::DealDamage()
//{
//	// 추가해야됨
// 
//}
//
//// 캐릭터 받는 데미지 공식
//float ACG_Character::TakeDamage()
//{
//	// 추가해야됨
//}

// 캐릭터 사망시 이벤트(게임오버 등)
void ACG_Character::OnDeath()
{
	// 추가해야됨
}

void ACG_Character::EquipWeapon()
{
	// 액터 생성 파라미터 설정
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	// Weapon 액터 소환
	AActor* NewWeapon = GetWorld()->SpawnActor<AActor>(StickyGunClass, GetActorLocation(), GetActorRotation(), SpawnParams);

	if (NewWeapon)
	{
		// 인터페이스 획득 시도
		IWeaponInterface* InterfaceWeapon = Cast<IWeaponInterface>(NewWeapon);

		if (InterfaceWeapon)
		{
			// 무기 부착
			(Cast<AActor>(InterfaceWeapon))->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("R_thumb_02_s_073Socket")); // 소켓에 부착
			EquippedWeapon = TScriptInterface<IWeaponInterface>(NewWeapon);
		}
	}
}

void ACG_Character::Fire()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// Critical Section 시작 (LastFireTime 접근 전에)
	FireTimeSection.Lock();

	if (EquippedWeapon.GetObject())
	{
		IWeaponInterface* Weapon = Cast<IWeaponInterface>(EquippedWeapon.GetObject());
		if (Weapon)
		{
			Weapon->Fire(); // 인터페이스를 통해 Fire 함수 직접 호출
		}
	}
	// Critical Section 종료 (LastFireTime 접근 후에)
	FireTimeSection.Unlock();
}