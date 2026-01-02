// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGridManager.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "MyGridCell.h"
#include "Kismet/GameplayStatics.h"
#include "Totem.h"
#include "Orb.h"
#include "Components/BoxComponent.h"
// Sets default values
AMyGridManager::AMyGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CellMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("CellMesh"));
	RootComponent = CellMesh;
	CellMesh->SetMobility(EComponentMobility::Static);
	Width = 100;
	Height = 100;
	CellSize = 100.0f;

	WallLeft = CreateDefaultSubobject<UBoxComponent>(TEXT("WallLeft"));
	WallRight = CreateDefaultSubobject<UBoxComponent>(TEXT("WallRight"));
	WallTop = CreateDefaultSubobject<UBoxComponent>(TEXT("WallTop"));
	WallBottom = CreateDefaultSubobject<UBoxComponent>(TEXT("WallBottom"));

	WallLeft->SetupAttachment(RootComponent);
	WallRight->SetupAttachment(RootComponent);
	WallTop->SetupAttachment(RootComponent);
	WallBottom->SetupAttachment(RootComponent);

	WallLeft->SetCollisionProfileName(TEXT("BlockAll"));
	WallRight->SetCollisionProfileName(TEXT("BlockAll"));
	WallTop->SetCollisionProfileName(TEXT("BlockAll"));
	WallBottom->SetCollisionProfileName(TEXT("BlockAll"));

	WallLeft->SetHiddenInGame(true);
	WallRight->SetHiddenInGame(true);
	WallTop->SetHiddenInGame(true);
	WallBottom->SetHiddenInGame(true);

	bGridReady = false;

	OrbCount = 5;
	TotemCount = 1;

	RockCount = 15;


}

// Called when the game starts or when spawned
void AMyGridManager::BeginPlay()
{
	Super::BeginPlay();
	CellMesh->ClearInstances();
	GridCells.Empty();

	const FVector GridOrigin = GetActorLocation();
	float TargetScale = CellSize / 100.0f;
	FVector InstanceScale(TargetScale, TargetScale, 0.1f);

	const float HalfTile = CellSize * 0.5f;

	for (int32 X = 0; X < Width; X++)
	{
		for (int32 Y = 0; Y < Height; Y++)
		{

			const FVector LocalPos((X * CellSize) + HalfTile, (Y * CellSize) + HalfTile, 0.f);
			const FVector WorldPos = GridOrigin + LocalPos;
			FTransform InstanceTransform;
			InstanceTransform.SetLocation(LocalPos);
			InstanceTransform.SetScale3D(InstanceScale);

			CellMesh->AddInstance(InstanceTransform);
			FMyGridCell Cell;
			Cell.WorldLocation = WorldPos;
			Cell.bIsOccupied = false;

			GridCells.Add(FIntPoint(X, Y), Cell);
		}
	}
	bGridReady = true;
	
	const float GridWidthWorld = Width * CellSize;
	const float GridHeightWorld = Height * CellSize;

	const float WallThickness = 50.f;
	const float WallHeight = 300.f;

	const FVector Origin = GetActorLocation();
	WallLeft->SetBoxExtent(FVector(WallThickness, GridHeightWorld * 0.5f, WallHeight));
	WallLeft->SetWorldLocation(Origin + FVector(
		-WallThickness,
		GridHeightWorld * 0.5f,
		WallHeight
	));

	WallRight->SetBoxExtent(FVector(WallThickness, GridHeightWorld * 0.5f, WallHeight));
	WallRight->SetWorldLocation(Origin + FVector(
		GridWidthWorld + WallThickness,
		GridHeightWorld * 0.5f,
		WallHeight
	));

	WallBottom->SetBoxExtent(FVector(GridWidthWorld * 0.5f, WallThickness, WallHeight));
	WallBottom->SetWorldLocation(Origin + FVector(
		GridWidthWorld * 0.5f,
		-WallThickness,
		WallHeight
	));

	WallTop->SetBoxExtent(FVector(GridWidthWorld * 0.5f, WallThickness, WallHeight));
	WallTop->SetWorldLocation(Origin + FVector(
		GridWidthWorld * 0.5f,
		GridHeightWorld + WallThickness,
		WallHeight
	));

}

void AMyGridManager::SpawnAtTile(const FIntPoint& Position, TSubclassOf<AActor> ActorToSpawn)
{
	if (!GridCells.Contains(Position)) return;

	FMyGridCell& TargetTile = GridCells[Position];

	if (TargetTile.bIsOccupied) return;

	TargetTile.bIsOccupied = true;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		ActorToSpawn,
		TargetTile.WorldLocation,
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (!SpawnedActor)
	{
		TargetTile.bIsOccupied = false;
		return;
	}

	FVector Origin, BoxExtent;
	SpawnedActor->GetActorBounds(false, Origin, BoxExtent);

	FVector AdjustedLocation = TargetTile.WorldLocation;
	AdjustedLocation.Z += BoxExtent.Z;
	SpawnedActor->SetActorLocation(AdjustedLocation);

	TargetTile.Occupant = SpawnedActor;
}

FIntPoint AMyGridManager::GetRandomFreeTile()
{
	TArray<FIntPoint> FreeTiles;

	for (const auto& Elem : GridCells)
	{
		if (!Elem.Value.bIsOccupied)
		{
			FreeTiles.Add(Elem.Key);
		}
	}

	if (FreeTiles.Num() > 0)
	{
		return FreeTiles[FMath::RandRange(0, FreeTiles.Num() - 1)];
	}

	return FIntPoint(-1, -1);
}


FVector AMyGridManager::GetTileWorldLocation(const FIntPoint& Tile) const
{
	if (GridCells.Contains(Tile))
	{
		return GridCells[Tile].WorldLocation;
	}
	return FVector::ZeroVector;
}
void AMyGridManager::OccupyTile(const FIntPoint& Tile, AActor* Actor)
{
	if (GridCells.Contains(Tile))
	{
		GridCells[Tile].bIsOccupied = true;
		GridCells[Tile].Occupant = Actor;
	}
}

void AMyGridManager::ResetGrid()
{
	for (auto& Elem : GridCells)
	{
		Elem.Value.bIsOccupied = false;
		Elem.Value.Occupant = nullptr;
	}

	TArray<AActor*> SpawnedActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), SpawnedActors);

	for (AActor* Actor : SpawnedActors)
	{
		if (Actor && Actor != this)
		{
			if (Actor->IsA(AOrb::StaticClass()) ||
				Actor->IsA(ATotem::StaticClass()) ||
				Actor->IsA(RockClass))
			{
				Actor->Destroy();
			}

		}
	}
}

void AMyGridManager::SpawnRandomActors(TSubclassOf<AActor> ActorClass, int32 Count)
{
	if (!ActorClass) return;

	for (int32 i = 0; i < Count; i++)
	{
		FIntPoint Tile = GetRandomFreeTile();
		if (Tile == FIntPoint(-1, -1))
		{
			return;
		}

		SpawnAtTile(Tile, ActorClass);
	}
}

void AMyGridManager::SpawnAllGameplayActors()
{
	SpawnRandomActors(OrbClass, OrbCount);
	SpawnRandomActors(TotemClass, TotemCount);
	SpawnRandomActors(RockClass, RockCount);
}
