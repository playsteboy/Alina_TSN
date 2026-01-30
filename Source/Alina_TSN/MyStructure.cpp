// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStructure.h"

// Sets default values
AMyStructure::AMyStructure()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyStructure::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyStructure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

