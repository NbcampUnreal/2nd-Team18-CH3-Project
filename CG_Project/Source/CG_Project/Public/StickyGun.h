#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "WeaponInterface.h"
#include "StickyGun.generated.h"

UCLASS()
class CG_PROJECT_API AStickyGun : public APawn, public IWeaponInterface
{
	GENERATED_BODY()

public:
	AStickyGun();
	
	virtual void Fire() override;
	
	UFUNCTION(BlueprintPure, Category = "Weapon")
	virtual float GetDamage() override;

protected:
	
	//무기 메시 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* WeaponMesh;

	//발사 위치
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* MuzzleLocation;

	//총알 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AStickyBullet> ProjectileClass;

	//총알 딜레이(끈끈이 바주카포!~)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	float FireDelay;

	// 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	float Damage;

	//최초 발사 시간(딜레이 계산용)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	float LastFireTime;

	// 발사 사운드
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	class USoundBase* FireSound;

	// 발사 애니메이션
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* FireAnimation;
	
	virtual void BeginPlay() override;
	
};
