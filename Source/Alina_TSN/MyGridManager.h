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

	UFUNCTION(BlueprintCallable)
	void SpawnAtTile(const FIntPoint& Position, TSubclassOf<AActor> ActorToSpawn);

	UFUNCTION(BlueprintCallable)
	FIntPoint GetRandomFreeTile();

	UFUNCTION(BlueprintCallable)
	void OccupyTile(const FIntPoint& Tile, AActor* Actor);

	UFUNCTION(BlueprintCallable)
	FVector GetTileWorldLocation(const FIntPoint& Tile) const;

	UFUNCTION(BlueprintCallable)
	void ResetGrid();

	UFUNCTION(BlueprintCallable)
	void SpawnRandomActors(TSubclassOf<AActor> ActorClass, int32 Count);

	UFUNCTION(BlueprintCallable)
	void SpawnAllGameplayActors();

	UFUNCTION(BlueprintCallable)
	FBox GetGridBounds() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	int32 Width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	int32 Height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	float CellSize;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInstancedStaticMeshComponent* CellMesh;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> OrbClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	int32 OrbCount;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> TotemClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	int32 TotemCount;


	bool bGridReady;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	TMap<FIntPoint, FMyGridCell> GridCells;

};
