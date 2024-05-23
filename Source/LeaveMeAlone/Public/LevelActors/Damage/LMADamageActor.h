// LeaveMeAlone game by MagicBit. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LMADamageActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class LEAVEMEALONE_API ALMADamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALMADamageActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	//������� �������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components");
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components");
	UStaticMeshComponent* StaticMesh;

	//�������� �������� �����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage");
	float Damage = 5.0f;
	//������ ������� �����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage");
	float SphereRadius = 100.0f;
};