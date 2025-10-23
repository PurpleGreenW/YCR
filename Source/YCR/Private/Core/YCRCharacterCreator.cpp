// Copyright YCR. All Rights Reserved.

#include "Core/YCRCharacterCreator.h"
#include "Character/CharacterPlayer.h"
#include "YCR/Public/GAS/YCRAbilitySystemComponent.h"
#include "YCR/Public/GAS/YCRAttributeSet.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"

// Static member initialization
TMap<EYCRCharacterClasses, FYCRCharacterCreationData> UYCRCharacterCreator::CachedCreationData;

UYCRCharacterCreator::UYCRCharacterCreator()
{
    // Load the character creation data table
    static ConstructorHelpers::FObjectFinder<UDataTable> CreationDataObject(
        TEXT("/Game/YCR/Data/DT_CharacterCreationData")
    );
    
    if (CreationDataObject.Succeeded())
    {
        CharacterCreationDataTable = CreationDataObject.Object;
    }
    
    // Set default player character class
    static ConstructorHelpers::FClassFinder<ACharacterPlayer> PlayerCharacterBPClass(
        TEXT("/Game/YCR/Blueprints/Characters/BP_CharacterPlayer")
    );
    
    if (PlayerCharacterBPClass.Succeeded())
    {
        PlayerCharacterClass = PlayerCharacterBPClass.Class;
    }
}

ACharacterPlayer* UYCRCharacterCreator::CreatePlayerCharacter(
    UObject* WorldContextObject,
    EYCRCharacterClasses CharacterClass,
    const FVector& SpawnLocation,
    const FRotator& SpawnRotation)
{
    // Validate world context
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("YCRCharacterCreator: Invalid world context"));
        return nullptr;
    }

    // Get the character creator CDO to access defaults
    const UYCRCharacterCreator* CreatorCDO = GetDefault<UYCRCharacterCreator>();
    if (!CreatorCDO->PlayerCharacterClass)
    {
        UE_LOG(LogTemp, Error, TEXT("YCRCharacterCreator: No player character class set"));
        return nullptr;
    }

    // Set spawn parameters
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    // Spawn the character
    ACharacterPlayer* NewCharacter = World->SpawnActor<ACharacterPlayer>(
        CreatorCDO->PlayerCharacterClass,
        SpawnLocation,
        SpawnRotation,
        SpawnParams
    );

    if (!NewCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("YCRCharacterCreator: Failed to spawn character"));
        return nullptr;
    }

    // Initialize with class data
    if (!InitializeCharacterWithClass(NewCharacter, CharacterClass))
    {
        UE_LOG(LogTemp, Warning, TEXT("YCRCharacterCreator: Failed to initialize character with class %s"), 
            *UEnum::GetValueAsString(CharacterClass));
        // Character spawned but not fully initialized - let it continue with defaults
    }

    return NewCharacter;
}

bool UYCRCharacterCreator::InitializeCharacterWithClass(
    ACharacterPlayer* Character,
    EYCRCharacterClasses CharacterClass)
{
    if (!Character)
    {
        return false;
    }

    // Get character creation data
    FYCRCharacterCreationData CreationData;
    if (!GetCharacterCreationData(CharacterClass, CreationData))
    {
        UE_LOG(LogTemp, Warning, TEXT("YCRCharacterCreator: No creation data found for class %s"), 
            *UEnum::GetValueAsString(CharacterClass));
        return false;
    }

    // Set the character's class
    Character->SetCharacterClass(CharacterClass);

    // Apply all character data
    ApplyBaseStats(Character, CreationData);
    ApplyCharacterVisuals(Character, CreationData);
    GrantStartingAbilities(Character, CreationData);

    // Set element affinity
    Character->SetCharacterElement(CreationData.ElementAffinity);

    UE_LOG(LogTemp, Log, TEXT("YCRCharacterCreator: Successfully initialized %s as %s"), 
        *Character->GetName(), 
        *CreationData.ClassName.ToString());

    return true;
}

bool UYCRCharacterCreator::GetCharacterCreationData(
    EYCRCharacterClasses CharacterClass,
    FYCRCharacterCreationData& OutData)
{
    // Check cache first
    if (CachedCreationData.Contains(CharacterClass))
    {
        OutData = CachedCreationData[CharacterClass];
        return true;
    }

    // Get the CDO to access the data table
    const UYCRCharacterCreator* CreatorCDO = GetDefault<UYCRCharacterCreator>();
    if (!CreatorCDO->CharacterCreationDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("YCRCharacterCreator: No character creation data table set"));
        return false;
    }

    // Convert enum to row name (e.g., Swordsman -> "Swordsman")
    FString RowName = UEnum::GetValueAsString(CharacterClass);
    RowName = RowName.Replace(TEXT("EYCRCharacterClass::"), TEXT(""));

    // Find the row in the data table
    FYCRCharacterCreationData* FoundData = CreatorCDO->CharacterCreationDataTable->FindRow<FYCRCharacterCreationData>(
        FName(*RowName),
        TEXT("YCRCharacterCreator::GetCharacterCreationData")
    );

    if (FoundData)
    {
        OutData = *FoundData;
        // Cache the data
        CachedCreationData.Add(CharacterClass, OutData);
        return true;
    }

    return false;
}

void UYCRCharacterCreator::ApplyCharacterVisuals(
    ACharacterPlayer* Character,
    const FYCRCharacterCreationData& CreationData)
{
    if (!Character)
    {
        return;
    }

    // Apply skeletal mesh if specified
    if (CreationData.CharacterMesh.IsValid())
    {
        if (USkeletalMesh* Mesh = CreationData.CharacterMesh.LoadSynchronous())
        {
            Character->GetMesh()->SetSkeletalMesh(Mesh);
        }
    }

    // Apply animation blueprint if specified
    if (CreationData.AnimationBlueprint.IsValid())
    {
        if (UClass* AnimClass = CreationData.AnimationBlueprint.LoadSynchronous())
        {
            Character->GetMesh()->SetAnimInstanceClass(AnimClass);
        }
    }
}

void UYCRCharacterCreator::GrantStartingAbilities(
    ACharacterPlayer* Character,
    const FYCRCharacterCreationData& CreationData)
{
    if (!Character)
    {
        return;
    }

    UYCRAbilitySystemComponent* ASC = Cast<UYCRAbilitySystemComponent>(Character->GetAbilitySystemComponent());
    if (!ASC)
    {
        return;
    }

    // Grant offensive ability
    if (CreationData.StartingOffensiveAbility)
    {
        FGameplayAbilitySpec OffensiveSpec(CreationData.StartingOffensiveAbility, 1, INDEX_NONE, Character);
        ASC->GiveAbility(OffensiveSpec);
    }

    // Grant passive ability
    if (CreationData.StartingPassiveAbility)
    {
        FGameplayAbilitySpec PassiveSpec(CreationData.StartingPassiveAbility, 1, INDEX_NONE, Character);
        ASC->GiveAbility(PassiveSpec);
    }
}

void UYCRCharacterCreator::ApplyBaseStats(
    ACharacterPlayer* Character,
    const FYCRCharacterCreationData& CreationData)
{
    if (!Character)
    {
        return;
    }

    // Apply movement speed
    if (UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement())
    {
        MovementComp->MaxWalkSpeed = CreationData.BaseMoveSpeed;
    }

    // Apply health through attribute set
    if (UYCRAttributeSet* AttributeSet = Character->GetAttributeSet())
    {
        AttributeSet->SetMaxHealth(CreationData.BaseHealth);
        AttributeSet->SetHealth(CreationData.BaseHealth);
    }
}