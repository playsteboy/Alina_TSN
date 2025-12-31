// Fill out your copyright notice in the Description page of Project Settings.


#include "Totem.h"
#include "Alina_TSNCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "Components/BoxComponent.h"
ATotem::ATotem()
{
    PrimaryActorTick.bCanEverTick = true;
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;
    Radius = 1.0f;
    MC = nullptr;
}

void ATotem::BeginPlay()
{
    Super::BeginPlay();
    MC = Cast<AAlina_TSNCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AAlina_TSNCharacter::StaticClass()));
}

void ATotem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!MC)
    {
        MC = Cast<AAlina_TSNCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AAlina_TSNCharacter::StaticClass()));
    }
    if (MC && MC->GetScore() == 5) {
        DetectInZone();
    }
}


void ATotem::DetectInZone() {
    TArray<FOverlapResult> Overlaps;
    FVector Position = GetActorLocation();
    FCollisionShape Shape = FCollisionShape::MakeSphere(Radius);

    bool bOverlap = GetWorld()->OverlapMultiByChannel(
        Overlaps,
        Position,
        FQuat::Identity,
        ECC_Pawn,
        Shape
    );

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    if (Overlaps.Num() > 0) {
        bIsInZone = true;
    }
    else {
        bIsInZone = false;
    }
    
	
}

