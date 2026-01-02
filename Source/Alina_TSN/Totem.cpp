// Fill out your copyright notice in the Description page of Project Settings.


#include "Totem.h"
#include "Alina_TSNCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"
#include "CollisionQueryParams.h"
#include "Components/BoxComponent.h"
ATotem::ATotem()
{
    PrimaryActorTick.bCanEverTick = true;
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

    bIsInZone = false;
    MC = nullptr;
}

void ATotem::BeginPlay()
{
    Super::BeginPlay();
    CollisionBox->SetCollisionProfileName(TEXT("Trigger"));
    CollisionBox->SetGenerateOverlapEvents(true);

    CollisionBox->OnComponentBeginOverlap.AddDynamic(
        this, &ATotem::OnZoneEnter);

    CollisionBox->OnComponentEndOverlap.AddDynamic(
        this, &ATotem::OnZoneExit);

    MC = Cast<AAlina_TSNCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AAlina_TSNCharacter::StaticClass()));
}

void ATotem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}


void ATotem::OnZoneEnter(
    UPrimitiveComponent*,
    AActor* OtherActor,
    UPrimitiveComponent*,
    int32,
    bool,
    const FHitResult&)
{
    AAlina_TSNCharacter* Player = Cast<AAlina_TSNCharacter>(OtherActor);
    if (Player && Player->GetScore() == 5)
    {
        bIsInZone = true;
        Player->CurrentTotem = this;

    }
}
void ATotem::OnZoneExit(
    UPrimitiveComponent*,
    AActor* OtherActor,
    UPrimitiveComponent*,
    int32)
{
    if (AAlina_TSNCharacter* Player = Cast<AAlina_TSNCharacter>(OtherActor))
    {
        bIsInZone = false;
        Player->CurrentTotem = nullptr;

    }
}



