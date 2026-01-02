// Copyright Epic Games, Inc. All Rights Reserved.

#include "Alina_TSNGameMode.h"
#include "Alina_TSNCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "MyGridManager.h"
#include "Alina_TSNCharacter.h"
#include "Kismet/GameplayStatics.h"

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
	bGameFinished = false;
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
        return;

    TArray<AActor*> FoundPawns;
    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        AAlina_TSNCharacter::StaticClass(),
        FoundPawns
    );

    for (AActor* Actor : FoundPawns)
    {
        Actor->Destroy();
    }

    bGameFinished = false;

    MyGridManager->ResetGrid();
    MyGridManager->SpawnAllGameplayActors();

    FIntPoint PlayerTile = MyGridManager->GetRandomFreeTile();
    FVector SpawnLocation = MyGridManager->GetTileWorldLocation(PlayerTile);

    if (APlayerController* PC = Cast<APlayerController>(NewPlayer))
    {
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
    }

    APawn* Pawn = SpawnDefaultPawnAtTransform(
        NewPlayer,
        FTransform(FRotator::ZeroRotator, SpawnLocation)
    );
    
    NewPlayer->Possess(Pawn);
    if (AAlina_TSNCharacter* MC = Cast<AAlina_TSNCharacter>(Pawn))
    {
        MC->ResetScore();
    }
    MyGridManager->OccupyTile(PlayerTile, Pawn);
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

			bGameFinished = true;
        }
    }
}
