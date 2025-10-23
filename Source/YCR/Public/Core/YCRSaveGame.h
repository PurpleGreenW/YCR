// YCRSaveGame.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Engine/DataTable.h"
#include "YCRSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FCardSaveData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, SaveGame)
    FName CardID;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 Count = 0;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 Level = 1; // Für Card-Upgrades (1-10 Rarität)
};

USTRUCT(BlueprintType)
struct FCharacterUnlockData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, SaveGame)
    FName CharacterClass;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 AdvancementLevel = 1; // 1 = Base, 2 = First Job Change, etc.

    UPROPERTY(BlueprintReadWrite, SaveGame)
    bool bIsUnlocked = false;
};

USTRUCT(BlueprintType)
struct FAchievementSaveData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, SaveGame)
    FName AchievementID;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    bool bIsCompleted = false;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    FDateTime CompletionTime;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 Progress = 0; // Für Achievements mit Fortschritt
};

USTRUCT(BlueprintType)
struct FMapProgressData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, SaveGame)
    FName MapName;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 HighestLevelCompleted = 0;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    float BestClearTime = 0.0f;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    TArray<FName> SecretsFound;
};

/**
 * Save Game Object for YCR
 * Stores all persistent player progression data
 */
UCLASS()
class YCR_API UYCRSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UYCRSaveGame();

    // =====================================================
    // Core Save Data
    // =====================================================

    /** Save slot name */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save Game")
    FString SaveSlotName;

    /** User index for this save */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save Game")
    int32 UserIndex;

    /** Save game version for migration support */
    UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Save Game")
    int32 SaveGameVersion = 1;

    /** Timestamp of last save */
    UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Save Game")
    FDateTime LastSaveTime;

    // =====================================================
    // Player Progress
    // =====================================================

    /** Total playtime in seconds */
    UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Progress")
    float TotalPlayTime = 0.0f;

    /** Total number of runs */
    UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Progress")
    int32 TotalRuns = 0;

    /** Successful runs */
    UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Progress")
    int32 SuccessfulRuns = 0;

    /** Highest map level unlocked */
    UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Progress")
    int32 HighestMapUnlocked = 1;

    /** Map-specific progress data */
    UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Progress")
    TMap<FName, FMapProgressData> MapProgress;

    // =====================================================
    // Resources
    // =====================================================

    /** Total essence (meta currency) */
    UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Resources")
    int32 TotalEssence = 0;

    /** Total gold earned across all runs */
    UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Resources")
    int32 TotalGoldEarned = 0;

    /** Current gold (if persisted between runs) */
    UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Resources")
    int32 CurrentGold = 0;

    // =====================================================
    // Unlocks
    // =====================================================

    /** Unlocked characters and their advancement levels */
    UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Unlocks")
    TArray<FCharacterUnlockData> UnlockedCharacters;

    /** Collected cards from monster drops */
    UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Unlocks")
    TArray<FCardSaveData> CollectedCards;

    /** Unlocked permanent upgrades */
    UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Unlocks")
    TMap<FName, int32> PermanentUpgrades;

    // =====================================================
    // Statistics
    // =====================================================

    /** Monster kill counts by type */
    UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Statistics")
    TMap<FName, int32> MonsterKillCounts;

    /** Total damage dealt */
    UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Statistics")
    int64 TotalDamageDealt = 0;

    /** Total damage taken */
    UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Statistics")
    int64 TotalDamageTaken = 0;

    /** Favorite character (most played) */
    UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Statistics")
    FName MostPlayedCharacter;

    /** Character play counts */
    UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Statistics")
    TMap<FName, int32> CharacterPlayCounts;

    // =====================================================
    // Achievements
    // =====================================================

    /** Completed achievements */
    UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Achievements")
    TArray<FAchievementSaveData> CompletedAchievements;

    /** Achievement progress tracking */
    UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Achievements")
    TMap<FName, int32> AchievementProgress;

    // =====================================================
    // Settings (Optional)
    // =====================================================

    /** Player preferences */
    UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Settings")
    TMap<FName, float> GameSettings;

    // =====================================================
    // Helper Functions
    // =====================================================

    /** Check if a character is unlocked */
    UFUNCTION(BlueprintPure, Category = "Save Game")
    bool IsCharacterUnlocked(const FName& CharacterClass) const;

    /** Get character advancement level */
    UFUNCTION(BlueprintPure, Category = "Save Game")
    int32 GetCharacterAdvancementLevel(const FName& CharacterClass) const;

    /** Get card count */
    UFUNCTION(BlueprintPure, Category = "Save Game")
    int32 GetCardCount(const FName& CardID) const;

    /** Check if achievement is completed */
    UFUNCTION(BlueprintPure, Category = "Save Game")
    bool IsAchievementCompleted(const FName& AchievementID) const;

    /** Calculate completion percentage */
    UFUNCTION(BlueprintPure, Category = "Save Game")
    float GetCompletionPercentage() const;

    /** Update play statistics */
    UFUNCTION(BlueprintCallable, Category = "Save Game")
    void UpdatePlayStatistics(float SessionTime, const FName& PlayedCharacter);
};