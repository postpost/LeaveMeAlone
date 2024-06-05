// LeaveMeAlone game by MagicBit. All rights reserved.


#include "Player/LMAPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Player/LMADefaultCharacter.h"
#include "Components/LMAHealthComponent.h"

void ALMAPlayerController::BeginPlay() 
{
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}

void ALMAPlayerController::BeginSpectatingState() 
{
	SetControlRotation(FRotator(-75.0f, 0.0f, 0.0f));
	Super::BeginSpectatingState();
}

void ALMAPlayerController::OnUnPossess() 
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Player is dead")));
	if (!OnDeadLevel.IsNull())
	{
		//UE_LOG(LogTemp, Warning, TEXT("OnDeadLevel: %s"), *OnDeadLevel.ToString()); 
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, OnDeadLevel);
	}
}
