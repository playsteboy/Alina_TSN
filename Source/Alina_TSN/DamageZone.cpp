// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageZone.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
ADamageZone::ADamageZone()
{
	PrimaryActorTick.bCanEverTick = false;
	DamageAmount = 1.f;
	Radius = 300.0f;
	DamageInterval = 2.5f;
	DamageSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DamageSphere"));
	RootComponent = DamageSphere;
	DamageSphere->SetSphereRadius(Radius);
	ZoneVisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZoneVisualMesh"));
	ZoneVisualMesh->SetupAttachment(DamageSphere);
	ZoneVisualMesh->SetCollisionProfileName(TEXT("NoCollision"));
    DamageSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DamageSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	DamageSphere->SetGenerateOverlapEvents(true);
	VisualDuration = 0.5f;
}

void ADamageZone::BeginPlay()
{
	Super::BeginPlay();
	if (DamageSphere)
	{
		DamageSphere->SetSphereRadius(Radius);
	}
	if (ZoneVisualMesh && ZoneVisualMesh->GetStaticMesh())
	{
		ZoneVisualMesh->SetVisibility(false);
		FBoxSphereBounds MeshBounds = ZoneVisualMesh->GetStaticMesh()->GetBounds();
		float MeshRadius = MeshBounds.SphereRadius;

		if (MeshRadius > 0.0f)
		{
			float ScaleFactor = Radius / MeshRadius;
			ZoneVisualMesh->SetRelativeScale3D(FVector(ScaleFactor));
		}
	}
	GetWorld()->GetTimerManager().SetTimer(
		DamageTimerHandle,
		this,
		&ADamageZone::ApplyZoneEffect,
		DamageInterval,
		true
	);
}
void ADamageZone::ApplyZoneEffect()
{
	if (!ZoneVisualMesh || !DamageSphere) return;

	ZoneVisualMesh->SetVisibility(true);

	TArray<AActor*> OverlappingActors;
	DamageSphere->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		APawn* VictimPawn = Cast<APawn>(Actor);
		if (VictimPawn && VictimPawn->IsPlayerControlled())
		{
			UGameplayStatics::ApplyDamage(
				Actor,
				DamageAmount,
				GetOwner() ? GetOwner()->GetInstigatorController() : nullptr,
				this,
				nullptr
			);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(
		HideTimerHandle,
		this,
		&ADamageZone::HideZoneVisual,
		VisualDuration,
		false
	);
}
void ADamageZone::HideZoneVisual()
{
	if (!IsValid(this)) return;
	if (!ZoneVisualMesh || !DamageSphere) return;

	ZoneVisualMesh->SetVisibility(false);
}

void ADamageZone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(HideTimerHandle);
}
