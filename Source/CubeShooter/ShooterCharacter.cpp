#include "ShooterCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ShooterPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Net/UnrealNetwork.h"
#include "CubeShooter/Widgets/PlayerNameWidget.h"
#include "Engine/World.h"

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
 
    UpdatePlayerNameWidget();
    
    
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

void AShooterCharacter::UpdatePlayerNameWidget()
{
    if (UUserWidget* Widget = NameWidgetComponent->GetUserWidgetObject())
    {
       
            if (auto NameWidget = Cast<UPlayerNameWidget>(Widget))
            {
                FString PlayerName = TEXT("Unknown");
                if (APlayerState* PS = GetPlayerState())
                {
                    PlayerName = PS->GetPlayerName();
                }
                NameWidget->SetPlayerName(PlayerName);
            }
        
    }
}

void AShooterCharacter::OnPlayerNameChanged()
{
    UpdatePlayerNameWidget();
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

void AShooterCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
}
