// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyZone.generated.h"

UCLASS()
class ALINA_TSN_API AMyZone : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AMyZone();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UFUNCTION()
	virtual void ApplyZoneEffect();

		
};
