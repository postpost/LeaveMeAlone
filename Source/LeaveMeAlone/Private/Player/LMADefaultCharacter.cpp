// LeaveMeAlone game by MagicBit. All rights reserved.


#include "Player/LMADefaultCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/LMAHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"



// Sets default values
ALMADefaultCharacter::ALMADefaultCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetUsingAbsoluteRotation(true); //данное условие не позволит нашей камере поворачиватьс€ в момент поворота персонажа, 
														//в жанре TDS камера статична над игроком
	SpringArmComponent->TargetArmLength = ArmLength;
	SpringArmComponent->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f)); //FRotator(Pitch (nose up/down_Y), Yaw(vertical rotation_X), Roll(Z))
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetFieldOfView(FOV);
	CameraComponent->bUsePawnControlRotation = false; //запрещает камере вращатьс€ относительно SpringArmComponent

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//Jump functions improvements
	GetCharacterMovement()->GravityScale = 1.5f;
	GetCharacterMovement()->JumpZVelocity = 650;

	//Health Component
	HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");

}

// Called when the game starts or when spawned
void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}

	//ѕодписка на делегат получени€ урона
	OnHealthChanged(HealthComponent->GetHealth()); //сначала вызываетс€ BeginPlay() у компонентов
	HealthComponent->OnHealthChanged.AddUObject(this, &ALMADefaultCharacter::OnHealthChanged);

	//ѕодписка на делегат FOnDeath из HealthComponent
	HealthComponent->OnDeath.AddUObject(this, &ALMADefaultCharacter::OnDeath);
}

// Called every frame
void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!HealthComponent->IsDead())
	{
		RotationPlayerOnCursor();
	}
}

// Called to bind functionality to input
void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("CameraZoom", this, &ALMADefaultCharacter::CameraZoom);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ALMADefaultCharacter::OnStartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ALMADefaultCharacter::OnStopJump);
}

void ALMADefaultCharacter::MoveForward(float Value) 
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void ALMADefaultCharacter::MoveRight(float Value) 
{
	AddMovementInput(GetActorRightVector(), Value);
}

void ALMADefaultCharacter::CameraZoom(float Value)
{
	float ZoomFactor = Value * ZoomMultiplier;
	SpringArmComponent->TargetArmLength += ZoomFactor;
	SpringArmComponent->TargetArmLength = FMath::Clamp(SpringArmComponent->TargetArmLength, 150, 1400);
}

void ALMADefaultCharacter::OnStartJump() 
{
	bPressedJump = true;
}

void ALMADefaultCharacter::OnStopJump() 
{
	bPressedJump = false;
}

void ALMADefaultCharacter::OnDeath() 
{
	CurrentCursor->DestroyRenderState_Concurrent();
	
	PlayAnimMontage(DeathMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}

void ALMADefaultCharacter::RotationPlayerOnCursor() 
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);

		float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
		SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));

		if (CurrentCursor)
		{
			CurrentCursor->SetWorldLocation(ResultHit.Location);
		}
	}
}

void ALMADefaultCharacter::OnHealthChanged(float NewHealth) 
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Health = %f"), NewHealth));
}

