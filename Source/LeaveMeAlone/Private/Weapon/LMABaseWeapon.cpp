// LeaveMeAlone game by MagicBit. All rights reserved.


#include "Weapon/LMABaseWeapon.h"
#include "Engine/SkeletalMesh.h"
#include "Kismet/GameplayStatics.h"


// собственная категория логирования
// для отображения в лог кол-ва боеприпасов
DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

// Sets default values
ALMABaseWeapon::ALMABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	SetRootComponent(WeaponComponent);

}

// Called when the game starts or when spawned
void ALMABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmoWeapon = AmmoWeapon;
}

void ALMABaseWeapon::Fire() 
{
	GetWorldTimerManager().SetTimer(FireTimeHandler, this, &ALMABaseWeapon::Shoot, ShootFrequency, true);
}

void ALMABaseWeapon::StopFire() 
{
	StopShooting();
}


void ALMABaseWeapon::StopShooting()
{
	GetWorldTimerManager().ClearTimer(FireTimeHandler);
}

void ALMABaseWeapon::DecrementBullets() 
{
	if (!IsCurrentClipEmpty())
		CurrentAmmoWeapon.Bullets--;
	//UE_LOG(LogWeapon, Display, TEXT("Bullets = %s"), *FString::FromInt(CurrentAmmoWeapon.Bullets));
	if (IsCurrentClipEmpty())
	{
		OnEmptyClipSignature.Broadcast();
	}
}

bool ALMABaseWeapon::IsCurrentClipEmpty() const
{
	return CurrentAmmoWeapon.Bullets == 0;
}

bool ALMABaseWeapon::IsCurrentClipFull() const
{
	return CurrentAmmoWeapon.Bullets == AmmoWeapon.Bullets;
}

void ALMABaseWeapon::ChangeClip() 
{
	//CurrentAmmoWeapon.Clips = AmmoWeapon.Clips;
	CurrentAmmoWeapon.Bullets = AmmoWeapon.Bullets;
}

void ALMABaseWeapon::Shoot() 
{
	const FTransform SocketTransform = WeaponComponent->GetSocketTransform(WeaponSocketName);
	const FVector TraceStart = SocketTransform.GetLocation();
	const FVector ShootDirection = SocketTransform.GetRotation().GetForwardVector();
	const FVector TraceEnd = TraceStart + ShootDirection * TraceDistance;

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Cyan, false, 1.0f, 0, 2.0f);

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);

	if (HitResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 24, FColor::Red, false, 1.0f);
	}
	DecrementBullets();
}


