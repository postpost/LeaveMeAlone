// LeaveMeAlone game by MagicBit. All rights reserved.


#include "Player/LMAPlayerController.h"

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
