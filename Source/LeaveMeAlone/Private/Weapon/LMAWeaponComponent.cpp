// LeaveMeAlone game by MagicBit. All rights reserved.


#include "Weapon/LMAWeaponComponent.h"
#include "Weapon/LMABaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/LMAReloadFinishedAnimNotify.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
ULMAWeaponComponent::ULMAWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnWeapon();

	InitAnimNotify();
}


// Called every frame
void ULMAWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULMAWeaponComponent::SpawnWeapon() 
{
	Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);
	if (Weapon)
	{
		//��������� �������
		Weapon->ChangeClip();
		// �������� ��������� ������, ������� � ACharacter � ��������� � ����������
		const auto Character = Cast<ACharacter>(GetOwner());
		
		if (Character)
		{
			FAttachmentTransformRules WeaponAttachmentRule(EAttachmentRule::SnapToTarget, false);
			Weapon->AttachToComponent(Character->GetMesh(), WeaponAttachmentRule, SocketName);
		}

		//Sign for a delegate OnEmptyClipSignature
		Weapon->OnEmptyClipSignature.AddUObject(this, &ULMAWeaponComponent::OnEmptyClip);
	}
}

void ULMAWeaponComponent::Fire() 
{
	if (Weapon && !AnimReloading)
		Weapon->Fire(); //�������� �-��� � ALMABaseWeapon
}

void ULMAWeaponComponent::StopFire()
{
	if (Weapon)
		Weapon->StopFire();
}

void ULMAWeaponComponent::InitAnimNotify() 
{
	if (!ReloadMontage)
		return;
	const auto NotifiesEvents = ReloadMontage->Notifies;
	for (auto NotifyEvent : NotifiesEvents)
	{
		auto ReloadFinished = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);
		if (ReloadFinished)
		{
			ReloadFinished->OnNotifyReloadFinished.AddUObject(this, &ULMAWeaponComponent::OnNotifyReloadFinished);
			break;
		}
	}
}

void ULMAWeaponComponent::OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh) 
{
	const auto Character = Cast<ACharacter>(GetOwner());
	if (Character->GetMesh() == SkeletalMesh)
	{
		AnimReloading = false;
	}
}

bool ULMAWeaponComponent::CanReload() const
{
	return !AnimReloading && !Weapon->IsCurrentClipFull();
}

void ULMAWeaponComponent::OnEmptyClip()
{
	CheckReload();
}

bool ULMAWeaponComponent::GetCurrentWeaponRef(FAmmoWeapon& AmmoWeapon) const
{
	if (Weapon)
	{
		AmmoWeapon = Weapon->GetCurrentAmmoWeapon(); //������ �� ��������� ������
		return true;
	}
	return false;
}

void ULMAWeaponComponent::Reload() 
{
	CheckReload();
}

void ULMAWeaponComponent::CheckReload() 
{
	if (!CanReload())
		return;
	// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("FullClip = %d"), Weapon->IsCurrentClipFull()));
	if (!Weapon->IsCurrentClipFull())
	{
		Weapon->ChangeClip();
		// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Bullets after Reloading = %i"),
		// Weapon->GetCurrentBulletNum()));
		AnimReloading = true;

		ACharacter* Character = Cast<ACharacter>(GetOwner());
		Character->PlayAnimMontage(ReloadMontage);
	}
}
