// LeaveMeAlone game by MagicBit. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaultCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ULMAHealthComponent;
class UAnimMontage;



UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALMADefaultCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//to get Health Component
	UFUNCTION()
	ULMAHealthComponent* GetHealthComponent() const { return HealthComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY()
	UDecalComponent* CurrentCursor = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.f, 40.f, 40.0f);

	UPROPERTY(EditAnywhere)
	float ZoomMultiplier = 10.0f;
	
	//Health Component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health");
	ULMAHealthComponent* HealthComponent;

	//Anim Montage (for Death)
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;


private:
	float YRotation = -75.0f; //поворот камеры по оси Y
	float ArmLength = 1400.0f; // длина штатива
	float FOV = 55.0f; //поле зрения камеры

	//Inputs
	void MoveForward(float Value); // axesX
	void MoveRight(float Value); //axesY

	void CameraZoom(float Value);
	void OnStartJump();
	void OnStopJump();

	//reaction on Death Delegate (in HealthComponent)
	void OnDeath();

	//cursor interpret on Death time
	void RotationPlayerOnCursor();

	//func for Damage Delegate
	void OnHealthChanged(float NewHealth);
};
