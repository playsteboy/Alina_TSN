// Copyright Epic Games, Inc. All Rights Reserved.

#include "Alina_TSNGameMode.h"
#include "Alina_TSNCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "MyGridManager.h"

AAlina_TSNGameMode::AAlina_TSNGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/Character/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
    ConstructorHelpers::FClassFinder<AMyGridManager> MyGridManagerBPClass(TEXT("/Game/ThirdPerson/Blueprints/Object/BP_MyGridManager"));
    if (MyGridManagerBPClass.Succeeded())
    {
        MyGridManagerClass = MyGridManagerBPClass.Class;
    }
}
void AAlina_TSNGameMode::BeginPlay()
{
    if (MyGridManagerClass)
    {
        MyGridManager = GetWorld()->SpawnActor<AMyGridManager>(MyGridManagerClass, FVector::ZeroVector, FRotator::ZeroRotator);
    }
    else {
        return;
    }
    Super::BeginPlay();

    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        RestartPlayer(PC);
    }

}
void AAlina_TSNGameMode::RestartPlayer(AController* NewPlayer)
{
    if (!NewPlayer || !MyGridManager || !MyGridManager->bGridReady)
    {
        return;
    }

    MyGridManager->ResetGrid();
    MyGridManager->SpawnAllGameplayActors();

    FIntPoint PlayerTile = MyGridManager->GetRandomFreeTile();
    FVector SpawnLocation = MyGridManager->GetTileWorldLocation(PlayerTile);

    APawn* Pawn = SpawnDefaultPawnAtTransform(
        NewPlayer,
        FTransform(FRotator::ZeroRotator, SpawnLocation)
    );

    if (Pawn)
    {
        NewPlayer->Possess(Pawn);
        MyGridManager->OccupyTile(PlayerTile, Pawn);
    }
}
void AAlina_TSNGameMode::FinishGame() {
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC)
        {
            PC->SetInputMode(FInputModeUIOnly());
            PC->bShowMouseCursor = true;

            PC->UnPossess();
        }
    }
}
