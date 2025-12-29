// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGridManager.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "MyGridCell.h"
// Sets default values
AMyGridManager::AMyGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CellMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("CellMesh"));
	RootComponent = CellMesh;
	CellMesh->SetMobility(EComponentMobility::Static);
	GridSizeX = 10;
	GridSizeY = 10;
	CellSize = 100.0f;

}

// Called when the game starts or when spawned
void AMyGridManager::BeginPlay()
{
	Super::BeginPlay();
	const FVector GridOrigin = GetActorLocation();
	float TargetScale = CellSize / 100.0f;
	FVector InstanceScale(TargetScale, TargetScale, 0.1f);
	const float HalfCell = CellSize * 0.5f;
	for(int32 X = 0; X < GridSizeX; X++)
	{
		for(int32 Y = 0; Y < GridSizeY; Y++)
		{
			const FVector CellLocation = GridOrigin + FVector(X * CellSize + HalfCell, Y * CellSize + HalfCell, 0.0f);
			FTransform InstanceTransform;
			InstanceTransform.SetLocation(CellLocation);
			InstanceTransform.SetScale3D(InstanceScale);
			CellMesh->AddInstance(InstanceTransform);
			FMyGridCell Cell;
			Cell.WorldLocation = CellLocation;
			Cell.bIsOccupied = false;

			GridCells.Add(FIntPoint(X, Y), Cell);
		}
	}
	
}

// Called every frame
void AMyGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

