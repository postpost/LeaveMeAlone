// LeaveMeAlone game by MagicBit. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaultCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ULMAHealthComponent;
class UAnimMontage;

class ULMAWeaponComponent;

UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALMADefaultCharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsSprinting = false;

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
	ULMAHealthComponent* GetHealthComponent() const { return HealthComponent;}

	//Weapon Component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	ULMAWeaponComponent* WeaponComponent;

	//check to SprintEnable
	UFUNCTION(BlueprintCallable)
	bool IsMoving(); // check for Sprint

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

	UPROPERTY(EditAnywhere, Category = "Sprint Settings")
	float SprintSpeed = 2000;
		
	UPROPERTY(EditAnywhere, Category = "Sprint Settings")
	float SprintAcceleration = 4000;

	UPROPERTY(EditAnywhere, Category = "Sprint Settings")
	float MaxStamina = 100;
		
	UPROPERTY(EditAnywhere, Category = "Sprint Settings")
	float StaminaDamage = 5.0f;

private:
	float YRotation = -75.0f; //������� ������ �� ��� Y
	float ArmLength = 1400.0f; // ����� �������
	float FOV = 55.0f; //���� ������ ������

	float InitialSpeed;
	float InitialAcceleration;

	float Stamina = 0.0f;

	bool IsStaminaRestored = false;

	//Inputs
	void MoveForward(float Value); // axesX
	void MoveRight(float Value); //axesY

	void CameraZoom(float Value);
	void OnStartJump();
	void OnStopJump();
	
	// Sprint
	void StartSprint();
	void StopSprint();

	//Stamina
	void DecreaseStamina();
	void RestoreStamina();

	

	//Timer to restore Stamina
	FTimerHandle StaminaRestoreTimerHandle;
	FTimerHandle StaminaDecreaseTimerHandle;

	//reaction on Death Delegate (in HealthComponent)
	void OnDeath();

	//cursor interpret on Death time
	void RotationPlayerOnCursor();

	//func for Damage Delegate
	void OnHealthChanged(float NewHealth);

};
