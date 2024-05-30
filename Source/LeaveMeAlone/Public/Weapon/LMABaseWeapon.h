// LeaveMeAlone game by MagicBit. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LMABaseWeapon.generated.h"

class USkeletalMeshComponent;

//delagate declaration
DECLARE_MULTICAST_DELEGATE(FOnEmptyClipSignature);

USTRUCT(BlueprintType)
struct FAmmoWeapon
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon");
	int32 Bullets;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon");
	int32 Clips;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon");
	bool Infinite;
};

UCLASS()
class LEAVEMEALONE_API ALMABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALMABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TraceDistance = 800.0f;

	void Shoot();
	void StopShooting();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Fire();
	void StopFire();

	void DecrementBullets();
	void ChangeClip(); //пополнение
	bool IsCurrentClipEmpty() const;

	int GetCurrentBulletNum() const { return CurrentAmmoWeapon.Bullets; }
	
	//Get USTRUCT for UI(Text_Binding)
	FAmmoWeapon GetCurrentAmmoWeapon() const { return CurrentAmmoWeapon; }

	UFUNCTION(BlueprintCallable)
	bool IsCurrentClipFull() const;

	FOnEmptyClipSignature OnEmptyClipSignature;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoWeapon AmmoWeapon {30, 1, true};
	
	//следит за текущим боеприпасом
	FAmmoWeapon CurrentAmmoWeapon;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	FName WeaponSocketName = "Muzzle";

	// Timer for Fire()
	FTimerHandle FireTimeHandler;
};
