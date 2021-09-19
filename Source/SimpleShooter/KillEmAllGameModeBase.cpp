// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameModeBase.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"

void AKillEmAllGameModeBase::PawnKilled(APawn* PawnKilled) {
	Super::PawnKilled(PawnKilled);

	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	if (PlayerController != nullptr) {
		EndGame(false);
	}

	for (AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld())) {
		if (!Controller->IsDead())
			return;
	}
	EndGame(true);
}

void AKillEmAllGameModeBase::EndGame(bool bPlayerHasWon) {
	for (AController* Controller : TActorRange<AController>(GetWorld())) {
		Controller->GameHasEnded(
			Controller->GetPawn(), 
			Controller->IsPlayerController() ? bPlayerHasWon : !bPlayerHasWon
		);
	}
}