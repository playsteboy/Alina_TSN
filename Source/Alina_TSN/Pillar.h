// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyStructure.h"
#include "Pillar.generated.h"

class ADamageZone; class UBoxComponent;

 
UCLASS()
class ALINA_TSN_API APillar : public AMyStructure
{
	GENERATED_BODY()

public:
	APillar();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Zone")
	TSubclassOf<ADamageZone> DamageZoneClass;

	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionBox;

	

	virtual void BeginPlay() override;
};
