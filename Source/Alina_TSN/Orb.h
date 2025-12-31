// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCollectible.h"
#include "Orb.generated.h"
class UBoxComponent;
/**
 * 
 */
UCLASS()
class ALINA_TSN_API AOrb : public AMyCollectible
{
	GENERATED_BODY()

public:
	AOrb();
	UFUNCTION()
	virtual void OverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 otherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionBox;

};
