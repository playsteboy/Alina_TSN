// Fill out your copyright notice in the Description page of Project Settings.


#include "Pillar.h"
#include "DamageZone.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"

APillar::APillar()
{
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;
}

void APillar::BeginPlay()
{
	Super::BeginPlay();

	if (DamageZoneClass)
	{
		FActorSpawnParameters Params;
		Params.Owner = this;

		ADamageZone* Zone = GetWorld()->SpawnActor<ADamageZone>(
			DamageZoneClass,
			GetActorLocation(),
			FRotator::ZeroRotator,
			Params
		);

		if (Zone)
		{
			Zone->AttachToActor(
				this,
				FAttachmentTransformRules::KeepWorldTransform
			);
		}
	}
}

