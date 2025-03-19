#include "CG_Character.h"
#include "CG_PlayerController.h"
#include "CG_GameState.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"

ACG_Character::ACG_Character()
{
	PrimaryActorTick.bCanEverTick = false;

	// ī�޶� ���� ���� ����
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	// ī�޶�
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	// �Ӹ� �� HP ���� ����
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	// �÷��̾� �̵��ӵ�
	NormalSpeed = 750.0f;
	SprintSpeedMultiplier = 2.0f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	// ĳ���� ü��
	MaxHealth = (CurrentLevel * 10.0f) + 90.0f;
	CurrentHealth = MaxHealth;

	// ĳ���� ���� 
	CurrentLevel = 1;
}

void ACG_Character::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHP();
}

// ĳ���� ���� ����
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
		}
	}
}

// �̵�
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

// ����(����)
void ACG_Character::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

// ����(����)
void ACG_Character::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

// ����
void ACG_Character::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

// �޸���(����)
void ACG_Character::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

// �޸���(����)
void ACG_Character::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

// ĳ���� �Ӹ� �� hp��
void ACG_Character::UpdateOverheadHP()
{
	// �߰��ؾߵ�
}

// ���� ü�� get�Լ�
float ACG_Character::GetHealth() const
{
	return CurrentHealth;
}

// ü�� ȸ�� ��Ŀ����
void ACG_Character::HealHealth(float Amount)
{
	// Clamp(�ּ�~�ִ� ü�� ������ ��, �ּ�ü��, �ִ�ü��)
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0f, MaxHealth);
	// ����� ���� �� ü�¹� ������ ������Ʈ
	UpdateOverheadHP();
}

//// ĳ���� ������ ����(���� ��� ������ ���� �����ؾ� ��)
//float ACG_Character::DealDamage()
//{
//	// �߰��ؾߵ�
// 
//}
//
//// ĳ���� �޴� ������ ����
//float ACG_Character::TakeDamage()
//{
//	// �߰��ؾߵ�
//}

// ĳ���� ����� �̺�Ʈ(���ӿ��� ��)
void ACG_Character::OnDeath()
{
	// �߰��ؾߵ�
}