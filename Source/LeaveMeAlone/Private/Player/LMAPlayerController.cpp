// LeaveMeAlone game by MagicBit. All rights reserved.


#include "Player/LMAPlayerController.h"

void ALMAPlayerController::BeginPlay() 
{
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}
