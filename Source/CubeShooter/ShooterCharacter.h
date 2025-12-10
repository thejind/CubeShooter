#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/WidgetComponent.h"
#include "ShooterCharacter.generated.h"


class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class ABulletProjectile;

UCLASS()
class CUBESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AShooterCharacter();

	
protected:
	virtual void BeginPlay() override;
	
	virtual void Tick( float DeltaTime ) override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerShoot(const FVector& Start, const FVector& End);
	
	void AwardScore();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	TSubclassOf<ABulletProjectile> BulletProjectileClass;
	
	// Enhanced Input Mapping Context
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	// Input Actions
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> FireAction;

	// Widget component to hold the player name widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* NameWidgetComponent;
 
	// The widget class to use for the player name (set in Blueprint or defaults)
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PlayerNameWidgetClass;
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetupLocalDisplayName();
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SetPlayerNameOnServer(const FString& NewName);
	
	UPROPERTY(ReplicatedUsing=OnRep_CustomPlayerName)
	FString CustomPlayerName;
	
	UFUNCTION()
	void OnRep_CustomPlayerName();
	
	FTimerHandle FWidgetValidationTimer;
	FTimerHandle FWidgetRotationTimer;
	
	UFUNCTION()
	void SetNameTagRotationToPlayer();
	
	// Input callbacks
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void JumpStarted(const FInputActionValue& Value);
	void JumpStopped(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);


private:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	FVector MuzzleOffset;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};