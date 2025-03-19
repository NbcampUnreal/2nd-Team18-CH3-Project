#include "CG_Character.h"
#include "CG_PlayerController.h"
#include "CG_GameState.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "WeaponInterface.h"
#include "StickyGun.h"

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

	if (StickyGunClass)
	{
		EquipWeapon(); // 무기 소환
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("StickyGunClass is not set!"));
	}
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
			//발사
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
			(Cast<AActor>(InterfaceWeapon))->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("L_thumb_02_s_047Socket")); // 소켓에 부착
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