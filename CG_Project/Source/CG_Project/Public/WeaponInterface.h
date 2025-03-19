
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"


UINTERFACE(MinimalAPI)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};


class CG_PROJECT_API IWeaponInterface
{
	GENERATED_BODY()

	
public:
	// 발사 함수
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire() = 0;

	// 데미지 반환 함수
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual float GetDamage() = 0;
};
