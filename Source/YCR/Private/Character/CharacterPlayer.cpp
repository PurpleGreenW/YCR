// Copyright YCR Project

#include "Character/CharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "YCR/Public/GAS/YCRAbilitySystemComponent.h"
#include "YCR/Public/GAS/YCRAttributeSet.h"
#include "Interfaces/IInteractableInterface.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"

ACharacterPlayer::ACharacterPlayer()
{
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
    GetCharacterMovement()->JumpZVelocity = 600.f;
    GetCharacterMovement()->AirControl = 0.2f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 800.0f; // The camera follows at this distance behind the character
    CameraBoom->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f)); // Isometric-style angle
    CameraBoom->bUsePawnControlRotation = false; // Don't rotate the arm based on the controller
    CameraBoom->bInheritPitch = false;
    CameraBoom->bInheritYaw = false;
    CameraBoom->bInheritRoll = false;

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    // Player-specific defaults
    CurrentExperience = 0.0f;
    ExperienceToNextLevel = 100.0f; // Base exp for level 2
    PickupRadius = 300.0f;
    CurrentGold = 0;
    CurrentEssence = 0;
}

void ACharacterPlayer::BeginPlay()
{
    Super::BeginPlay();

    // Add Input Mapping Context
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
        
        YCRPlayerController = PlayerController;
    }

    // Start periodic item collection
    GetWorldTimerManager().SetTimer(ItemCollectionTimerHandle, this, &ACharacterPlayer::CollectNearbyItems, 0.1f, true);
}

void ACharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Set up action bindings
    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Jumping
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

        // Moving
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACharacterPlayer::Move);

        // Looking
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACharacterPlayer::Look);

        // Interacting
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACharacterPlayer::Interact);
    }
}

void ACharacterPlayer::Move(const FInputActionValue& Value)
{
    // Input is a Vector2D
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // Find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // Get forward vector
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        
        // Get right vector 
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // Add movement 
        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void ACharacterPlayer::Look(const FInputActionValue& Value)
{
    // Input is a Vector2D
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // Add yaw and pitch input to controller
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void ACharacterPlayer::Interact()
{
    CheckForInteractables();
}

void ACharacterPlayer::CollectExperience(float Amount)
{
    CurrentExperience += Amount;

    // Check for level up
    while (CurrentExperience >= ExperienceToNextLevel)
    {
        CurrentExperience -= ExperienceToNextLevel;
        LevelUp();
    }
}

void ACharacterPlayer::LevelUp()
{
    // Increase character level
    SetCharacterLevel(CharacterLevel + 1);

    // Calculate next level requirement (exponential growth)
    ExperienceToNextLevel = FMath::RoundToFloat(ExperienceToNextLevel * 1.2f);

    // Trigger level up event for UI/selection
    OnLevelUp();

    // Log for debugging
    UE_LOG(LogTemp, Log, TEXT("Player leveled up to level %d!"), CharacterLevel);
}

void ACharacterPlayer::CheckForInteractables()
{
    // Sphere trace for interactables
    FVector StartLocation = GetActorLocation();
    FVector EndLocation = StartLocation; // Same as start for sphere trace

    TArray<FHitResult> HitResults;
    FCollisionShape CollisionShape;
    CollisionShape.SetSphere(200.0f); // Interaction radius

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    bool bHit = GetWorld()->SweepMultiByChannel(
        HitResults,
        StartLocation,
        EndLocation,
        FQuat::Identity,
        ECC_GameTraceChannel1, // Custom interaction channel
        CollisionShape,
        QueryParams
    );

    if (bHit)
    {
        for (const FHitResult& Hit : HitResults)
        {
            if (Hit.GetActor() && Hit.GetActor()->Implements<UIInteractableInterface>())
            {
                // Prüfe ob Interaktion möglich ist
                if (IIInteractableInterface::Execute_CanInteract(Hit.GetActor(), this))
                {
                    // Führe Interaktion aus
                    IIInteractableInterface::Execute_Interact(Hit.GetActor(), this);
                    
                    // Optional: Zeige Prompt
                    FText Prompt = IIInteractableInterface::Execute_GetInteractionPrompt(Hit.GetActor());
                    UE_LOG(LogTemp, Log, TEXT("Interacting: %s"), *Prompt.ToString());
                }
                break;
            }
        }
    }

    // Debug visualization
#if ENABLE_DRAW_DEBUG
    DrawDebugSphere(GetWorld(), StartLocation, 200.0f, 12, FColor::Yellow, false, 0.5f);
#endif
}

void ACharacterPlayer::CollectNearbyItems()
{
    // Sphere overlap for automatic pickup
    TArray<FOverlapResult> OverlapResults;
    FCollisionShape CollisionShape;
    CollisionShape.SetSphere(PickupRadius);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    bool bOverlap = GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        GetActorLocation(),
        FQuat::Identity,
        ECC_GameTraceChannel2, // Custom pickup channel
        CollisionShape,
        QueryParams
    );

    if (bOverlap)
    {
        for (const FOverlapResult& Overlap : OverlapResults)
        {
            if (Overlap.GetActor())
            {
                // Attempt to collect the item
                // This would interface with your pickup system
                // For now, just destroy the actor and add experience
                if (Overlap.GetActor()->ActorHasTag("Experience"))
                {
                    AddExperience(10.0f); // Base experience value
                    Overlap.GetActor()->Destroy();
                }
                else if (Overlap.GetActor()->ActorHasTag("Gold"))
                {
                    AddGold(5); // Base gold value
                    Overlap.GetActor()->Destroy();
                }
            }
        }
    }
}

void ACharacterPlayer::AddExperience(float Amount)
{
    CollectExperience(Amount);
}

void ACharacterPlayer::AddGold(int32 Amount)
{
    CurrentGold += Amount;
    OnItemCollected(TEXT("Gold"), Amount);
}

void ACharacterPlayer::AddEssence(int32 Amount)
{
    CurrentEssence += Amount;
    OnItemCollected(TEXT("Essence"), Amount);
}

float ACharacterPlayer::GetExperiencePercent() const
{
    return ExperienceToNextLevel > 0.0f ? CurrentExperience / ExperienceToNextLevel : 0.0f;
}

void ACharacterPlayer::InitializeAbilitySystem()
{
    Super::InitializeAbilitySystem();

    // Additional player-specific ability system initialization
    if (AbilitySystemComponent)
    {
        // Set up gameplay ability delegates for player
        AbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.AddUObject(
            this, &ACharacterPlayer::OnGameplayEffectApplied
        );
    }
}

void ACharacterPlayer::GrantDefaultAbilities()
{
    Super::GrantDefaultAbilities();

    // Grant additional player-specific abilities based on class
    // This would be expanded based on your job class system
}

// Helper function for effect notifications (add to class declaration if needed)
void ACharacterPlayer::OnGameplayEffectApplied(UAbilitySystemComponent* Source, 
    const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveHandle)
{
    // Handle visual feedback for buffs/debuffs
}