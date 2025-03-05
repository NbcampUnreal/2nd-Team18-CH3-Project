#include "CG_Character.h"
// #include "플레이어 컨트롤러 헤더"
// #include "게임스테이트 헤더"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"

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
	NormalSpeed = 750.0f;
	SprintSpeedMultiplier = 2.0f;
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
}

// 캐릭터 조작 관련
void ACG_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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