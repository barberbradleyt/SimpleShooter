// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "ShooterCharacter.h"
//#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

void AShooterAIController::BeginPlay() {
	Super::BeginPlay();

	if (AIBehavior != nullptr) {
		RunBehaviorTree(AIBehavior);
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
}

void AShooterAIController::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
/*
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn != nullptr) {
		if (LineOfSightTo(PlayerPawn)) {
			GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
			GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
		}
		else {
			GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
		}
	}
*/
}

bool AShooterAIController::IsDead() const {
	AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
	// return AShooterCharacter::IsDead() if non-null, otherwise default to true
	return ControlledCharacter != nullptr ? ControlledCharacter->IsDead() : true;
}