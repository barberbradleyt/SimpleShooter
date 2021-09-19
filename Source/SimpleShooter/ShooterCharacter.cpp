// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	UE_LOG(LogTemp, Warning, TEXT("Character::CanCrouch=%s"), (CanCrouch() ? TEXT("true") : TEXT("false")));

	//Instantiating Class Components
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));

	//Attach the Spring Arm to the Character's Skeletal Mesh Component
	SpringArmComp->SetupAttachment(GetCapsuleComponent());

	//Attach the Camera to the SpringArmComponent
	CameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepRelativeTransform);

	//Setting default properties of the SpringArmComp
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagMaxDistance = CameraLagMaxDistance;
	SpringArmComp->TargetArmLength = SpringArmLength;
	SpringArmComp->SocketOffset = SpringArmSocketOffset;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0.0;
}

float AShooterCharacter::GetHealthPercent() const 
{
	return Health / MaxHealth;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);

	// Mouse Look
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AShooterCharacter::LookRight);

	// Controller Look
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);

	// Actions
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("ToggleCrouch"), EInputEvent::IE_Pressed, this, &AShooterCharacter::ToggleCrouch);
	PlayerInputComponent->BindAction(TEXT("SwitchCameraShoulder"), EInputEvent::IE_Pressed, this, &AShooterCharacter::SwitchCameraShoulder);
	PlayerInputComponent->BindAction(TEXT("Zoom"), EInputEvent::IE_Pressed, this, &AShooterCharacter::ZoomAim);
	PlayerInputComponent->BindAction(TEXT("Zoom"), EInputEvent::IE_Released, this, &AShooterCharacter::UnZoomAim);
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health = Health - DamageToApply;

	UE_LOG(LogTemp, Warning, TEXT("Health left = %f"), Health);

	if (IsDead()) {
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr) {
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return DamageToApply;
}


///////////////////// Movement and Look functions ///////////////////////////

// Movement
void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}
void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}
void AShooterCharacter::ToggleCrouch() {
	UE_LOG(LogTemp, Warning, TEXT("Crouching/Uncrouching"));
	UE_LOG(LogTemp, Warning, TEXT("Character::CanCrouch=%s"), (CanCrouch() ? TEXT("true") : TEXT("false")));
	UE_LOG(LogTemp, Warning, TEXT("old Character::IsCrouching=%s"), (bIsCrouched ? TEXT("true") : TEXT("false")));
	bIsCrouched ? UnCrouch() : Crouch();
	UE_LOG(LogTemp, Warning, TEXT("new Character::IsCrouching=%s"), (bIsCrouched ? TEXT("true") : TEXT("false")));
}

// Mouse Look
void AShooterCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(-1 * AxisValue);
}
void AShooterCharacter::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

// Controller Look
void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}
void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}
/////////////////////////////////////////////////////////////////////////////

////////////////////////////// Action Functions /////////////////////////////

void AShooterCharacter::Shoot() {
	Gun->PullTrigger();
}
void AShooterCharacter::SwitchCameraShoulder() {
	UE_LOG(LogTemp, Warning, TEXT("SwitchingCameraShoulder"));
	if (SpringArmComp != nullptr) {
		SpringArmComp->SocketOffset = SpringArmComp->SocketOffset * FVector(1.0f, -1.0f, 1.0f);
	}
}
void AShooterCharacter::ZoomAim() {
	UE_LOG(LogTemp, Warning, TEXT("Zoom pressed"));
	if (SpringArmComp != nullptr) {
		SpringArmComp->TargetArmLength = ZoomedSpringArmLength;
	}
}
void AShooterCharacter::UnZoomAim() {
	UE_LOG(LogTemp, Warning, TEXT("Zoom released"));
	if (SpringArmComp != nullptr) {
		SpringArmComp->TargetArmLength = SpringArmLength;
	}
}
/////////////////////////////////////////////////////////////////////////////


