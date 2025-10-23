// Copyright YCR. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "YCR/Public/Enums/EYCRCharacterClasses.h"
#include "YCR/Public/Enums/EYCRElementTypes.h"
#include "Engine/DataTable.h"
#include "YCRCharacterCreator.generated.h"

// Forward declarations
class ACharacterPlayer;
class UYCRAbilitySystemComponent;
class UGameplayAbility;
class UGameplayEffect;
class USkeletalMesh;
class UAnimBlueprint;

/**
 * Character creation data structure
 * Contains all necessary data for creating a character with specific job class
 */
USTRUCT(BlueprintType)
struct FYCRCharacterCreationData : public FTableRowBase
{
    GENERATED_BODY()

    /** Display name of the character class */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
    FText ClassName;

    /** Description of the character class */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
    FText ClassDescription;

    /** Base health for this class */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float BaseHealth = 100.0f;

    /** Base movement speed */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float BaseMoveSpeed = 400.0f;

    /** Starting offensive ability */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
    TSubclassOf<UGameplayAbility> StartingOffensiveAbility;

    /** Starting passive ability */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
    TSubclassOf<UGameplayAbility> StartingPassiveAbility;

    /** Starting element affinity */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
    EYCRElementTypes ElementAffinity = EYCRElementTypes::Neutral;

    /** Skeletal mesh for this class */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visuals")
    TSoftObjectPtr<USkeletalMesh> CharacterMesh;

    /** Animation blueprint for this class */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visuals")
    TSoftClassPtr<UAnimBlueprint> AnimationBlueprint;

    /** Icon for UI representation */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    UTexture2D* ClassIcon;

    FYCRCharacterCreationData()
    {
        BaseHealth = 100.0f;
        BaseMoveSpeed = 400.0f;
        ElementAffinity = EYCRElementTypes::Neutral;
    }
};

/**
 * Character Creator utility class
 * Handles the creation and initialization of player characters based on job class
 */
UCLASS(BlueprintType, Blueprintable)
class YCR_API UYCRCharacterCreator : public UObject
{
    GENERATED_BODY()

public:
    UYCRCharacterCreator();

    /**
     * Create a new player character with specified class
     * @param World - World context
     * @param CharacterClass - The job class to create
     * @param SpawnLocation - Where to spawn the character
     * @param SpawnRotation - Initial rotation
     * @return The created character or nullptr on failure
     */
    UFUNCTION(BlueprintCallable, Category = "YCR|Character Creation", meta = (WorldContext = "WorldContextObject"))
    static ACharacterPlayer* CreatePlayerCharacter(
        UObject* WorldContextObject,
        EYCRCharacterClasses CharacterClass,
        const FVector& SpawnLocation,
        const FRotator& SpawnRotation
    );

    /**
     * Initialize character with class-specific data
     * @param Character - Character to initialize
     * @param CharacterClass - The job class data to apply
     * @return Success of initialization
     */
    UFUNCTION(BlueprintCallable, Category = "YCR|Character Creation")
    static bool InitializeCharacterWithClass(
        ACharacterPlayer* Character,
        EYCRCharacterClasses CharacterClass
    );

    /**
     * Get character creation data for a specific class
     * @param CharacterClass - The job class to query
     * @param OutData - The character creation data
     * @return Whether data was found
     */
    UFUNCTION(BlueprintCallable, Category = "YCR|Character Creation")
    static bool GetCharacterCreationData(
        EYCRCharacterClasses CharacterClass,
        FYCRCharacterCreationData& OutData
    );

    /**
     * Apply visual customization to character
     * @param Character - Character to customize
     * @param CreationData - Data containing visual information
     */
    static void ApplyCharacterVisuals(
        ACharacterPlayer* Character,
        const FYCRCharacterCreationData& CreationData
    );

    /**
     * Grant starting abilities to character
     * @param Character - Character to grant abilities to
     * @param CreationData - Data containing ability information
     */
    static void GrantStartingAbilities(
        ACharacterPlayer* Character,
        const FYCRCharacterCreationData& CreationData
    );

    /**
     * Apply base stats to character
     * @param Character - Character to apply stats to
     * @param CreationData - Data containing stat information
     */
    static void ApplyBaseStats(
        ACharacterPlayer* Character,
        const FYCRCharacterCreationData& CreationData
    );

protected:
    /** Data table containing character creation data for all classes */
    UPROPERTY(EditDefaultsOnly, Category = "YCR|Character Creation")
    UDataTable* CharacterCreationDataTable;

    /** Default player character class to spawn */
    UPROPERTY(EditDefaultsOnly, Category = "YCR|Character Creation")
    TSubclassOf<ACharacterPlayer> PlayerCharacterClass;

private:
    /** Cache of loaded character creation data */
    static TMap<EYCRCharacterClasses, FYCRCharacterCreationData> CachedCreationData;
};