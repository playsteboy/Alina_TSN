// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Alina_TSNCharacter.generated.h"

class USpringArmComponent; class ATotem; class AAlina_TSNGameMode; class AMyGridManager;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AAlina_TSNCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	float GetInteractionPercentage() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float InteractTime;
	


public:
	AAlina_TSNCharacter();

	UFUNCTION(BlueprintCallable)
	void IncreaseScoreBy(int num);

	UFUNCTION()
	void ResetScore();

	UFUNCTION(BlueprintCallable)
	int GetScore() const;

	UFUNCTION(BlueprintCallable)
	int GetHealth() const;

	UFUNCTION(BlueprintCallable)
	void SetHealth(int value);

	UFUNCTION(BlueprintCallable)
	void OnInteractionComplete();

	ATotem* CurrentTotem;

	UFUNCTION(BlueprintCallable)
	bool IsInteractionCompleted() const;

	UFUNCTION()
	int GetMaxHealth();

	UFUNCTION()
	int GetTotalOrbs() const;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void Interact();

	void StopInteract();

			

protected:

	virtual void BeginPlay() override;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	int Score;

	int Health;
	
	FTimerHandle InteractionTimerHandle;

	AAlina_TSNGameMode* GM;

	AMyGridManager* GridManager;
	bool bInteractionCompleted;

	int MaxHealth;

	int TotalOrbs;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

