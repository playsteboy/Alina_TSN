// Copyright Epic Games, Inc. All Rights Reserved.

#include "Alina_TSNGameMode.h"
#include "Alina_TSNCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAlina_TSNGameMode::AAlina_TSNGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
