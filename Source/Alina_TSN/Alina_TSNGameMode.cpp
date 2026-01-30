// Copyright Epic Games, Inc. All Rights Reserved.

#include "Alina_TSNGameMode.h"
#include "Alina_TSNCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "MyGridManager.h"
#include "Alina_TSNCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
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

	bPlayerWon = false;
	TimeRemaining = 99.f;
	TotalScore = 0.f;
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

    TimeRemainingSave = TimeRemaining;
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
	bPlayerWon = false;
	TimeRemaining = TimeRemainingSave;
    TotalScore = TimeRemaining;
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
		MC->SetHealth(3);
    }
    MyGridManager->OccupyTile(PlayerTile, Pawn);
    if (GetWorldTimerManager().IsTimerActive(TimerHandle_Mission))
    {
        GetWorldTimerManager().ClearTimer(TimerHandle_Mission);
    }
    GetWorldTimerManager().SetTimer(
        TimerHandle_Mission,
        this,
        &AAlina_TSNGameMode::UpdateTimer,
        1.0f,
        true
    );
}



void AAlina_TSNGameMode::FinishGame() {
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC)
        {
            APawn* CurrentPawn = PC->GetPawn();

            if (CurrentPawn)
            {
                AAlina_TSNCharacter* MyCharacter = Cast<AAlina_TSNCharacter>(CurrentPawn);
                if (MyCharacter)
                {
 
					bPlayerWon = (MyCharacter->IsInteractionCompleted());
                }
            }
            else
            {
                bPlayerWon = false;
            }
            PC->SetInputMode(FInputModeUIOnly());
            PC->bShowMouseCursor = true;
            PC->UnPossess();

            bGameFinished = true;

            if (GetWorldTimerManager().IsTimerActive(TimerHandle_Mission))
            {
                GetWorldTimerManager().ClearTimer(TimerHandle_Mission);
            }
        }
    }
}

void AAlina_TSNGameMode::UpdateTimer()
{
    if (bGameFinished)
        return;
    TimeRemaining -= 1.0f;
    TotalScore = TimeRemaining;
    if (TimeRemaining <= 0.0f)
    {
        FinishGame();
        
    }
}

float AAlina_TSNGameMode::GetTimeRemaining() const
{
    if (GetWorldTimerManager().IsTimerActive(TimerHandle_Mission))
    {
        return TimeRemaining;
    }
    return 0.0f;
}

float AAlina_TSNGameMode::CalculateFinalScore()
{
    AAlina_TSNCharacter* PlayerCharacter = nullptr;
    for (TActorIterator<AAlina_TSNCharacter> It(GetWorld()); It; ++It)
    {
        PlayerCharacter = *It;
        break;
    }

    if (PlayerCharacter)
    {
        float MaxHealth = static_cast<float>(PlayerCharacter->GetMaxHealth());
        float CurrentHealth = static_cast<float>(PlayerCharacter->GetHealth());
		float WinScore = bPlayerWon ? 100.0f : 0.0f;
        float HealthPerc = 0.0f;
        float TimePerc = 0.0f;
        if (bPlayerWon) {
            HealthPerc = (MaxHealth > 0.0f) ? (CurrentHealth / MaxHealth) * 100.0f : 0.0f;
            if (TimeRemainingSave > 0.0f)
            {
                TimePerc = (TimeRemaining / TimeRemainingSave) * 100.0f;
                TimePerc = FMath::Clamp(TimePerc, 0.0f, 100.0f);
            }
        }
        
        float TotalOrbs = static_cast<float>(PlayerCharacter->GetTotalOrbs());
        float CurrentScore = static_cast<float>(PlayerCharacter->GetScore());

        float ItemsPerc = 0.0f;
        if (TotalOrbs > 0.0f)
        {
            ItemsPerc = (CurrentScore / TotalOrbs) * 100.0f;
        }
        float WeightedScore = (HealthPerc * 0.25f) + (TimePerc * 0.25f) + (ItemsPerc * 0.25f) + (WinScore * 0.25f);
        return WeightedScore;
    }

    return 0.0f;
}