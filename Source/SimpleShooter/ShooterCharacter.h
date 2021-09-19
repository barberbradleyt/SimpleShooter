// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly)
	float CameraLagMaxDistance = 2.0f;

	UPROPERTY(EditDefaultsOnly)
	float SpringArmLength = 225.0f;

	UPROPERTY(EditDefaultsOnly)
	float ZoomedSpringArmLength = 50.0f;

	UPROPERTY(EditDefaultsOnly)
	FVector SpringArmSocketOffset = FVector(0.0f, 58.0f, 65.0f);

public:
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float TakeDamage(
		float DamageAmount, 
		struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator, 
		AActor* DamageCauser) override;

	void Shoot();

private:
	// Movement (hardware independant)
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void ToggleCrouch();
	void SwitchCameraShoulder();
	void ZoomAim();
	void UnZoomAim();

	// Mouse Look
	void LookUp(float AxisValue);
	void LookRight(float AxisValue);

	// Controller Look
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);


	//Actions
	UPROPERTY(EditAnywhere)
	float RotationRate = 10;
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;

	UPROPERTY()
	AGun* Gun;
};
