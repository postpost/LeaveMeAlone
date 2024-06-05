// LeaveMeAlone game by MagicBit. All rights reserved.


#include "Player/LMAPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Player/LMADefaultCharacter.h"
#include "Components/LMAHealthComponent.h"

void ALMAPlayerController::BeginPlay() 
{
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;

	auto CurrentCharacter = Cast<ALMADefaultCharacter>(GetOwner());
	if (CurrentCharacter)
	{
		ULMAHealthComponent* CurrentHealthComponent = CurrentCharacter->GetHealthComponent();
		CurrentHealthComponent->OnDeath.AddUObject(this, &ALMAPlayerController::OnUnPossess);
	}
}

void ALMAPlayerController::BeginSpectatingState() 
{
	SetControlRotation(FRotator(-75.0f, 0.0f, 0.0f));
	Super::BeginSpectatingState();
}

void ALMAPlayerController::OnUnPossess() 
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Player is dead")));
	if (OnDeadLevel !=nullptr)
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), OnDeadLevel);
	
	}
}
