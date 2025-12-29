// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MyGridCell.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyGridManager.generated.h"
class UInstancedStaticMeshComponent;
UCLASS()
class ALINA_TSN_API AMyGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyGridManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	int32 GridSizeX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	int32 GridSizeY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	float CellSize;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInstancedStaticMeshComponent* CellMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TMap<FIntPoint, FMyGridCell> GridCells;

};
