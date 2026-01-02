// Copyright Epic Games, Inc. All Rights Reserved.

#include "Alina_TSNCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Alina_TSNGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Totem.h"
#include "MyGridManager.h"
DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AAlina_TSNCharacter

AAlina_TSNCharacter::AAlina_TSNCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(96.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 750.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->MaxWalkSpeed = 750.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1200.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input
void AAlina_TSNCharacter::BeginPlay() {
	Super::BeginPlay();
	CurrentTotem = Cast<ATotem>(UGameplayStatics::GetActorOfClass(GetWorld(), ATotem::StaticClass()));
}
void AAlina_TSNCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AAlina_TSNCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAlina_TSNCharacter::Move);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AAlina_TSNCharacter::Interact);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AAlina_TSNCharacter::StopInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Canceled, this, &AAlina_TSNCharacter::StopInteract);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AAlina_TSNCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AAlina_TSNCharacter::IncreaseScoreBy(int num) {
	Score += num;
}

int AAlina_TSNCharacter::GetScore() const {
	return Score;
}

void AAlina_TSNCharacter::Interact()
{
	if (!CurrentTotem) return;

	if (GetWorldTimerManager().IsTimerActive(InteractionTimerHandle))
		return;

	if (Score == 5 && CurrentTotem->bIsInZone) {
		GetCharacterMovement()->DisableMovement();

		AAlina_TSNGameMode* GM = GetWorld()->GetAuthGameMode<AAlina_TSNGameMode>();
		if (GM)
		{
			GetWorldTimerManager().SetTimer(
				InteractionTimerHandle,
				GM,
				&AAlina_TSNGameMode::FinishGame,
				5.f,
				false
			);
		}
	}
}



float AAlina_TSNCharacter::GetInteractionPercentage() const
{
	if (GetWorldTimerManager().IsTimerActive(InteractionTimerHandle))
	{
		float Remaining = GetWorldTimerManager().GetTimerRemaining(InteractionTimerHandle);
		float TotalTime = 5.0f;
		return (TotalTime - Remaining) / TotalTime;
	}
	return 0.0f;
}

void AAlina_TSNCharacter::StopInteract()
{
	if (GetWorldTimerManager().IsTimerActive(InteractionTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(InteractionTimerHandle);
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}
void AAlina_TSNCharacter::ResetScore() {
	Score = 0;
}