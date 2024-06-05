// LeaveMeAlone game by MagicBit. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LMAPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LEAVEMEALONE_API ALMAPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void BeginSpectatingState() override;

public:
	UPROPERTY(BlueprintReadWrite)
	TSoftObjectPtr<UWorld> OnDeadLevel;

private:
	UFUNCTION()
	virtual void OnUnPossess() override;
};
