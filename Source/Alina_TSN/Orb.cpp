// Fill out your copyright notice in the Description page of Project Settings.


#include "Orb.h"
#include "Alina_TSNCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"
#include "Components/BoxComponent.h"
AOrb::AOrb() {
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;
    CollisionBox->SetGenerateOverlapEvents(true);

    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AOrb::OverlapBegin);
}
void AOrb::OverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 otherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	if (OtherActor && (OtherActor != this))
	{
		AAlina_TSNCharacter* Character = Cast<AAlina_TSNCharacter>(OtherActor);
		if (Character)
		{
			Character->IncreaseScoreBy(1);
			Destroy();
		}
	}
}