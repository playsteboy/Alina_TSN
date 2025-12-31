// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Totem.generated.h"
class AAlina_TSNCharacter; class UBoxComponent;
UCLASS()
class ALINA_TSN_API ATotem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATotem();

	UPROPERTY(BlueprintReadWrite)
	float Radius;

	UPROPERTY(BlueprintReadOnly)
	bool bIsInZone;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionBox;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void DetectInZone();

	AAlina_TSNCharacter* MC;



};
