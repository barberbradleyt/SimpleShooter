// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "ShooterCharacter.h"

UBTTask_Shoot::UBTTask_Shoot() {
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	UE_LOG(LogTemp, Warning, TEXT("Executiong Shoot"))
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("No Owner Found"))
		return EBTNodeResult::Failed;
	}

	AShooterCharacter* Character = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("No Character Found"))
		return EBTNodeResult::Failed;
	}

	// Shoot gun
	UE_LOG(LogTemp, Warning, TEXT("Shooting"))
	Character->Shoot();

	return EBTNodeResult::Succeeded;
}