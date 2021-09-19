// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    void BeginPlay() override;
    void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

private:
    //HUD elements
    UPROPERTY(EditAnywhere)
    TSubclassOf<class UUserWidget> HUDClass;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class UUserWidget> WinScreenClass;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class UUserWidget> LoseScreenClass;

    UPROPERTY()
    UUserWidget* HUD;

    //Timer
    UPROPERTY(EditAnywhere)
    float RestartDelay = 5;

    FTimerHandle RestartTimer;
};
