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

	// ???? ???? ???? ????
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	// ????
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	// ??? ?? HP ???? ????
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	// ??????? ??????
	NormalSpeed = 350.0f;
	SprintSpeedMultiplier = 1.8f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	// ?????? ???
	MaxHealth = (CurrentLevel * 10.0f) + 90.0f;
	CurrentHealth = MaxHealth;

	// ?????? ???? 
	CurrentLevel = 1;
}

void ACG_Character::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHP();

	if (StickyGunClass)
	{
		EquipWeapon(); // ???? ???
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("StickyGunClass is not set!"));
	}
}

// ?????? ???? ????
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

// ???
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

// ????(????)
void ACG_Character::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

// ????(????)
void ACG_Character::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

// ????
void ACG_Character::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

// ?????(????)
void ACG_Character::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

// ?????(????)
void ACG_Character::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}


// ?????? (?????)
void ACG_Character::Roll(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Roll Activated!"));
	}
}

// ?????? ??? ?? hp??
void ACG_Character::UpdateOverheadHP()
{
	// ???????
}

// ???? ??? get???
float ACG_Character::GetHealth() const
{
	return CurrentHealth;
}

// ??? ??? ????????
void ACG_Character::HealHealth(float Amount)
{
	// Clamp(???~??? ??? ?????? ??, ??????, ??????)
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0f, MaxHealth);
	// ????? ???? ?? ????? ?????? ???????
	UpdateOverheadHP();
}

//// ?????? ?????? ????(???? ??? ?????? ???? ??????? ??)
//float ACG_Character::DealDamage()
//{
//	// ???????
// 
//}
//
//// ?????? ??? ?????? ????
//float ACG_Character::TakeDamage()
//{
//	// ???????
//}

// ?????? ????? ????(??????? ??)
void ACG_Character::OnDeath()
{
	// ???????
}

void ACG_Character::EquipWeapon()
{
	// ???? ???? ?????? ????
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	// Weapon ???? ???
	AActor* NewWeapon = GetWorld()->SpawnActor<AActor>(StickyGunClass, GetActorLocation(), GetActorRotation(), SpawnParams);

	if (NewWeapon)
	{
		// ????????? ??? ???
		IWeaponInterface* InterfaceWeapon = Cast<IWeaponInterface>(NewWeapon);

		if (InterfaceWeapon)
		{
			// ???? ????
			(Cast<AActor>(InterfaceWeapon))->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("R_thumb_02_s_073Socket")); // ????? ????
			EquippedWeapon = TScriptInterface<IWeaponInterface>(NewWeapon);
		}
	}
}

void ACG_Character::Fire()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// Critical Section ???? (LastFireTime ???? ????)
	FireTimeSection.Lock();
	
	if (EquippedWeapon.GetObject())
	{
		IWeaponInterface* Weapon = Cast<IWeaponInterface>(EquippedWeapon.GetObject());
		if (Weapon)
		{
			Weapon->Fire(); // ??????????? ???? Fire ??? ???? ???
		}
	}
	// Critical Section ???? (LastFireTime ???? ????)
	FireTimeSection.Unlock();
}