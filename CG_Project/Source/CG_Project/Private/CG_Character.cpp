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

	// Ä«¸Þ¶ó Á¶Á¤ ¸·´ë °ü·Ã
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	// Ä«¸Þ¶ó
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	// ¸Ó¸® À§ HP °ü·Ã À§Á¬
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	// ÇÃ·¹ÀÌ¾î ÀÌµ¿¼Óµµ
	NormalSpeed = 350.0f;
	SprintSpeedMultiplier = 1.8f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	// Ä³¸¯ÅÍ Ã¼·Â
	MaxHealth = (CurrentLevel * 10.0f) + 90.0f;
	CurrentHealth = MaxHealth;

	// Ä³¸¯ÅÍ ·¹º§ 
	CurrentLevel = 1;
}

void ACG_Character::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHP();

	if (StickyGunClass)
	{
		EquipWeapon(); // ¹«±â ¼ÒÈ¯
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("StickyGunClass is not set!"));
	}
}

// Ä³¸¯ÅÍ Á¶ÀÛ °ü·Ã
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

// ÀÌµ¿
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

// Á¡ÇÁ(½ÃÀÛ)
void ACG_Character::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

// Á¡ÇÁ(¸ØÃã)
void ACG_Character::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

// ½ÃÁ¡
void ACG_Character::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

// ´Þ¸®±â(½ÃÀÛ)
void ACG_Character::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

// ´Þ¸®±â(¸ØÃã)
void ACG_Character::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}


// ±¸¸£±â (¿ìÅ¬¸¯)
void ACG_Character::Roll(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Roll Activated!"));
	}
}

// Ä³¸¯ÅÍ ¸Ó¸® À§ hp¹Ù
void ACG_Character::UpdateOverheadHP()
{
	// Ãß°¡ÇØ¾ßµÊ
}

// ÇöÀç Ã¼·Â getÇÔ¼ö
float ACG_Character::GetHealth() const
{
	return CurrentHealth;
}

// Ã¼·Â È¸º¹ ¸ÅÄ¿´ÏÁò
void ACG_Character::HealHealth(float Amount)
{
	// Clamp(ÃÖ¼Ò~ÃÖ´ë Ã¼·Â »çÀÌÀÇ °ª, ÃÖ¼ÒÃ¼·Â, ÃÖ´ëÃ¼·Â)
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0f, MaxHealth);
	// °è»êÀÌ ³¡³­ µÚ Ã¼·Â¹Ù À§Á¬¿¡ ¾÷µ¥ÀÌÆ®
	UpdateOverheadHP();
}

//// Ä³¸¯ÅÍ µ¥¹ÌÁö °ø½Ä(·¹º§ ºñ·Ê µ¥¹ÌÁö Áõ°¡ Æ÷ÇÔÇØ¾ß ÇÔ)
//float ACG_Character::DealDamage()
//{
//	// Ãß°¡ÇØ¾ßµÊ
// 
//}
//
//// Ä³¸¯ÅÍ ¹Þ´Â µ¥¹ÌÁö °ø½Ä
//float ACG_Character::TakeDamage()
//{
//	// Ãß°¡ÇØ¾ßµÊ
//}

// Ä³¸¯ÅÍ »ç¸Á½Ã ÀÌº¥Æ®(°ÔÀÓ¿À¹ö µî)
void ACG_Character::OnDeath()
{
	// Ãß°¡ÇØ¾ßµÊ
}

void ACG_Character::EquipWeapon()
{
	// ¾×ÅÍ »ý¼º ÆÄ¶ó¹ÌÅÍ ¼³Á¤
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	// Weapon ¾×ÅÍ ¼ÒÈ¯
	AActor* NewWeapon = GetWorld()->SpawnActor<AActor>(StickyGunClass, GetActorLocation(), GetActorRotation(), SpawnParams);

	if (NewWeapon)
	{
		// ÀÎÅÍÆäÀÌ½º È¹µæ ½Ãµµ
		IWeaponInterface* InterfaceWeapon = Cast<IWeaponInterface>(NewWeapon);

		if (InterfaceWeapon)
		{
			// ¹«±â ºÎÂø
			(Cast<AActor>(InterfaceWeapon))->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("R_thumb_02_s_073Socket")); // ¼ÒÄÏ¿¡ ºÎÂø
			EquippedWeapon = TScriptInterface<IWeaponInterface>(NewWeapon);
		}
	}
}

void ACG_Character::Fire()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// Critical Section ½ÃÀÛ (LastFireTime Á¢±Ù Àü¿¡)
	FireTimeSection.Lock();
	
	if (EquippedWeapon.GetObject())
	{
		IWeaponInterface* Weapon = Cast<IWeaponInterface>(EquippedWeapon.GetObject());
		if (Weapon)
		{
			Weapon->Fire(); // ÀÎÅÍÆäÀÌ½º¸¦ ÅëÇØ Fire ÇÔ¼ö Á÷Á¢ È£Ãâ
		}
	}
<<<<<<< Updated upstream
	// Critical Section Á¾·á (LastFireTime Á¢±Ù ÈÄ¿¡)
=======
	// Critical Section ì¢…ë£Œ (LastFireTime ì ‘ê·¼ í›„ì—)
>>>>>>> Stashed changes
	FireTimeSection.Unlock();
}