#include "ShooterCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ShooterGameInstance.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ShooterPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Net/UnrealNetwork.h"
#include "CubeShooter/Widgets/PlayerNameWidget.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

AShooterCharacter::AShooterCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
    bReplicates = true;
    SetReplicateMovement(true);
    FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FPSCameraComponent->SetupAttachment(GetCapsuleComponent());
    FPSCameraComponent->SetRelativeLocation(FVector(0, 0, 64.f));
    FPSCameraComponent->bUsePawnControlRotation = true;
    
    GetMesh()->SetOwnerNoSee(false);

    MuzzleOffset = FVector(100.f, 0.f, 0.f);
    
    // Create WidgetComponent and attach it (attach to head or root)
    NameWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameWidgetComponent"));
    NameWidgetComponent->SetupAttachment(GetRootComponent()); 
    NameWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // Screen space for flat UI
    NameWidgetComponent->SetDrawSize(FVector2D(200.f, 50.f));
    NameWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 110.f)); // Adjust above head height
    NameWidgetComponent->SetOwnerNoSee(true);
    
}


void AShooterCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PC = Cast<APlayerController>(Controller))
    {
        if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                if (DefaultMappingContext)
                {
                    Subsystem->AddMappingContext(DefaultMappingContext, 0);
                }
            }
        }
    }
    
    if (PlayerNameWidgetClass)
    {
        NameWidgetComponent->SetWidgetClass(PlayerNameWidgetClass);
    }
    
    SetupLocalDisplayName();
    GetWorldTimerManager().SetTimer(FWidgetRotationTimer, this, &ThisClass::SetNameTagRotationToPlayer, 0.1f, true, 0.1f);
    
}

void AShooterCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AShooterCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AShooterCharacter, CustomPlayerName);
}

void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

    if (MoveAction)
    {
        EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Move);
    }

    if (LookAction)
    {
        EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Look);
    }

    if (JumpAction)
    {
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &AShooterCharacter::JumpStarted);
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &AShooterCharacter::JumpStopped);
    }

    if (FireAction)
    {
        EnhancedInput->BindAction(FireAction, ETriggerEvent::Started, this, &AShooterCharacter::Fire);
    }
}


void AShooterCharacter::SetupLocalDisplayName()
{
    if (!IsLocallyControlled())
        return;
    
    if (UShooterGameInstance* SGI = Cast<UShooterGameInstance>(GetGameInstance()))
    {
        SetPlayerNameOnServer(SGI->PendingDisplayName);
        GEngine->AddOnScreenDebugMessage(
            -1, 10.0f, FColor::Green,
            FString::Printf(TEXT("Local Player Name %s"), *SGI->PendingDisplayName)
        );
    }
        
}

void AShooterCharacter::SetPlayerNameOnServer_Implementation(const FString& NewName)
{
    GEngine->AddOnScreenDebugMessage(
            -1, 10.0f, FColor::Green,
                FString::Printf(TEXT("Server Player Name %s"), *NewName)
        );
    if (CustomPlayerName != NewName)
    {
        CustomPlayerName = NewName;
        // Force replication update immediately if needed
        OnRep_CustomPlayerName();
        return;
    }
    CustomPlayerName = NewName;
    GEngine->AddOnScreenDebugMessage(
            -1, 10.0f, FColor::Red,
                FString::Printf(TEXT("Server Player Name %s"), *CustomPlayerName)
        );
}

void AShooterCharacter::OnRep_CustomPlayerName()
{
    if (UUserWidget* NameWidget =  NameWidgetComponent->GetWidget())
    {
        if (UPlayerNameWidget* PlayerNameWidget = Cast<UPlayerNameWidget>(NameWidget))
        {
            GEngine->AddOnScreenDebugMessage(
            -1, 10.0f, FColor::Green,
            FString::Printf(TEXT("ABC"))
                );
            PlayerNameWidget->SetDisplayName(CustomPlayerName);
            if (FWidgetValidationTimer.IsValid())
            {
                GetWorldTimerManager().ClearTimer(FWidgetValidationTimer);
            }
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(
            -1, 10.0f, FColor::Green,
            FString::Printf(TEXT("Widget Not Valid")));
            GetWorldTimerManager().SetTimer(FWidgetValidationTimer, this, &ThisClass::OnRep_CustomPlayerName, 0.1f, true, 0.1f);
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(
            -1, 10.0f, FColor::Green,
            FString::Printf(TEXT("Widget Component Not Valid"))
            );
        GetWorldTimerManager().SetTimer(FWidgetValidationTimer, this, &ThisClass::OnRep_CustomPlayerName, 0.1f, true, 0.1f);
    }
    
        
}

void AShooterCharacter::SetNameTagRotationToPlayer()
{
    if (!IsLocallyControlled() &&NameWidgetComponent)
    {
        ACharacter* LocalCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        if (LocalCharacter)
        {
            // Widget current location
            FVector WidgetLocation = NameWidgetComponent->GetComponentLocation();
            FVector TargetLocation = LocalCharacter->GetActorLocation();
 
            // Calculate rotation to look at local player
            FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(WidgetLocation, TargetLocation);
 
            // Lock pitch/roll if desired, usually name tags rotate only on Yaw
            LookAtRotation.Pitch = 0.f;
            LookAtRotation.Roll = 0.f;
 
            NameWidgetComponent->SetWorldRotation(LookAtRotation);
        }
    }
}

void AShooterCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    if (Controller)
    {
        AddMovementInput(GetActorForwardVector(), MovementVector.Y);
        AddMovementInput(GetActorRightVector(), MovementVector.X);
    }
}

void AShooterCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisValue = Value.Get<FVector2D>();
    if (Controller)
    {
        AddControllerYawInput(LookAxisValue.X);
        AddControllerPitchInput(-LookAxisValue.Y);
    }
}

void AShooterCharacter::JumpStarted(const FInputActionValue& Value)
{
    Jump();
}

void AShooterCharacter::JumpStopped(const FInputActionValue& Value)
{
    StopJumping();
}

void AShooterCharacter::Fire(const FInputActionValue& Value)
{
    
}




