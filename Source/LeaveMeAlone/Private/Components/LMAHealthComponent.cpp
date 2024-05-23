// LeaveMeAlone game by MagicBit. All rights reserved.


#include "Components/LMAHealthComponent.h"

// Sets default values for this component's properties
ULMAHealthComponent::ULMAHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void ULMAHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	OnHealthChanged.Broadcast(Health);
	//Broadcast on Delegate OnTakeAnyDamage
	AActor* OwnerComponent = GetOwner();
	if (OwnerComponent)
	{
		OwnerComponent->OnTakeAnyDamage.AddDynamic(this, &ULMAHealthComponent::OnTakeAnyDamage);
	}
}

bool ULMAHealthComponent::IsDead() const
{
	return Health <=0.0f;
}

bool ULMAHealthComponent::AddHealth(float NewHealth)
{
	if (IsDead() || IsHealthFull()) return false;
	Health = FMath::Clamp(Health + NewHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health); //�������� �� ���������
	return true;
}

bool ULMAHealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

void ULMAHealthComponent::OnTakeAnyDamage(
	AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsDead())
		return;
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
	if (IsDead())
	{
		OnDeath.Broadcast();
	}
}

