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

protected:
	virtual void BeginPlay() override;

private:
	virtual void RestartPlayer(AController* NewPlayer) override;

	AMyGridManager* MyGridManager;

	
};



