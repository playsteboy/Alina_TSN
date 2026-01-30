// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyZone.h"
#include "DamageZone.generated.h"

class USphereComponent; class UStaticMeshComponent;
 
 
UCLASS()
class ALINA_TSN_API ADamageZone : public AMyZone
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Zone")
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Zone")
	float Radius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage Zone")
	USphereComponent* DamageSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Zone")
	float DamageInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Zone")
	UStaticMeshComponent* ZoneVisualMesh;

	UPROPERTY(EditAnywhere, Category = "Damage Zone")
	float VisualDuration;

	ADamageZone();
	
	virtual void ApplyZoneEffect() override;


protected:
	virtual void BeginPlay() override;

	FTimerHandle DamageTimerHandle;

	void HideZoneVisual();

	FTimerHandle HideTimerHandle;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
