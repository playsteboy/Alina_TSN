// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Alina_TSNGameMode.generated.h"
class AMyGridManager;
UCLASS(minimalapi)
class AAlina_TSNGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAlina_TSNGameMode();

	UPROPERTY(EditAnywhere, Category = "Setup")
	TSubclassOf<AMyGridManager> MyGridManagerClass;


	UFUNCTION(BlueprintCallable)
	void FinishGame();

	virtual void RestartPlayer(AController* NewPlayer) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bGameFinished;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	float TimeRemaining;

	UFUNCTION(BlueprintCallable)
	float GetTimeRemaining() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bPlayerWon;

	UPROPERTY(BlueprintReadOnly, Category = "Score")
	float TotalScore;

	UFUNCTION(BlueprintCallable, Category = "Score")
	float CalculateFinalScore();
protected:
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle_Mission;

	void UpdateTimer();

private:

	AMyGridManager* MyGridManager;

	float TimeRemainingSave;

	float TimeElapsed;

	
};



