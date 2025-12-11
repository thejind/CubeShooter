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
#include "PhysicsCube.h"
#include "ShooterPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Net/UnrealNetwork.h"
#include "CubeShooter/Widgets/PlayerNameWidget.h"
#include "Engine/World.h"
#include "BulletProjectile.h"
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
    
    //Timer To Setup Other player's nametag on to face the owning client Player Character
    //not using tick but FTimerHandle
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
        EnhancedInput->BindAction(FireAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Fire);
    }
}

void AShooterCharacter::AwardScore()
{
    AShooterPlayerState* PS = GetPlayerState<AShooterPlayerState>();
    if (PS && HasAuthority())
    {
        PS->AddPlayerScore(1);
    }   
}


void AShooterCharacter::ServerShoot_Implementation(const FVector& Start, const FVector& End)
{
    // 1. Perform line trace for instant hit detection and gameplay logic
    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
 
    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        if (APhysicsCube* Cube = Cast<APhysicsCube>(Hit.GetActor()))
        {
            Cube->OnShot(GetPlayerState());
        }
    }
 
    // 2. Spawn replicated projectile for visual feedback
    FVector ShootDirection = (End - Start).GetSafeNormal();
 
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
 
    ABulletProjectile* Projectile = GetWorld()->SpawnActor<ABulletProjectile>(
        BulletProjectileClass,
        Start,
        ShootDirection.Rotation(),
        SpawnParams
    );
 
    if (Projectile)
    {
        Projectile->FireInDirection(ShootDirection);
    }
}

bool AShooterCharacter::ServerShoot_Validate(const FVector& Start, const FVector& End)
{
    // Confirm the caller is controlled by a valid player controller
    if (!GetController())
    {
        return false;
    }
    
    return true;
}


void AShooterCharacter::SetupLocalDisplayName()
{
    if (!IsLocallyControlled())
        return;
    
    if (UShooterGameInstance* SGI = Cast<UShooterGameInstance>(GetGameInstance()))
    {
        SetPlayerNameOnServer(SGI->PendingDisplayName);
    }
        
}

void AShooterCharacter::SetPlayerNameOnServer_Implementation(const FString& NewName)
{
    
    if (CustomPlayerName != NewName)
    {
        CustomPlayerName = NewName;
        
        // Force replication update immediately
        OnRep_CustomPlayerName();
        // propagate to PlayerState on server
        if (HasAuthority())  // Make sure running on server
        {
            if (AShooterPlayerState* PS = GetPlayerState<AShooterPlayerState>())
            {
                PS->SetPlayerDisplayName(CustomPlayerName);
            }
        }
        return;
    }
    
    CustomPlayerName = NewName;
}

void AShooterCharacter::OnRep_CustomPlayerName()
{
    if (UUserWidget* NameWidget =  NameWidgetComponent->GetWidget())
    {
        if (UPlayerNameWidget* PlayerNameWidget = Cast<UPlayerNameWidget>(NameWidget))
        {
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
    UE_LOG(LogTemp, Warning, TEXT("Fire input value: %d"), Value.Get<bool>());
    if (Value.Get<bool>())  // Boolean trigger input (pressed)
    {
        FVector Start = FPSCameraComponent->GetComponentLocation(); 
        FVector End = Start + (FPSCameraComponent->GetForwardVector() * 2000.f);
        ServerShoot(Start, End);
    }
}




