// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ShooterCharacter.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	Mesh->SetupAttachment(Root);
}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));
	
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) return;

	FHitResult HitResult;
	FVector ShotDirection;
	if (GunTrace(HitResult, ShotDirection)) {
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, HitResult.Location, ShotDirection.Rotation());
		FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
		AActor* ActorHit = HitResult.GetActor();
		if (ActorHit == nullptr) return; //Nothing Hit
		if (ActorHit == OwnerController->GetPawn()) return; //Self Hit
		else { //Actor Hit
			UE_LOG(LogTemp, Warning, TEXT("Gun hit: %s"), *ActorHit->GetName());

			AShooterCharacter* CharacterHit = Cast<AShooterCharacter>(ActorHit);
			if (CharacterHit == nullptr) {
				UE_LOG(LogTemp, Warning, TEXT("Env Hit"));
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EnvImpactParticle, HitResult.Location, ShotDirection.Rotation());
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), EnvImpactSound, HitResult.Location);
				return;
			}

			UE_LOG(LogTemp, Warning, TEXT("Character Hit"));
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActorImpactParticle, HitResult.Location, ShotDirection.Rotation());
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ActorImpactSound, HitResult.Location);
			ActorHit->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(FHitResult& HitResult, FVector& ShotDirection) {
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) return false;

	//Get ViewPoint location and rotation
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	OwnerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

	//Set shot direction
	ShotDirection = -ViewPointRotation.Vector();

	//Trace Gun fire and play effects if hit occurs
	FVector MaxHitLocation = ViewPointLocation + ViewPointRotation.Vector() * MaxRange;

	//Collision detection should ignore the calling Gun instance
	//as well as the ShooterCharacter instance owning this Gun instance
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(
		HitResult,
		ViewPointLocation,
		MaxHitLocation,
		ECollisionChannel::ECC_GameTraceChannel1,
		Params);
}

AController* AGun::GetOwnerController() {
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;
	
	return OwnerPawn->GetController();
}