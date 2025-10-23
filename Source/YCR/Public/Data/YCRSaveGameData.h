#pragma once

#include "CoreMinimal.h"
#include "YCRSaveGameData.generated.h"

/**
 * Player progression data that persists between runs
 */
USTRUCT(BlueprintType)
struct YCR_API FPlayerProgressData
{
    GENERATED_BODY()

    /** Highest map level unlocked */
    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 HighestMapUnlocked = 1;

    /** Highest level reached in any run */
    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 HighestLevelReached = 1;

    /** Total essence currency */
    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 TotalEssence = 0;

    /** Unlocked characters and their advancement levels */
    UPROPERTY(BlueprintReadWrite, SaveGame)
    TMap<FName, int32> UnlockedCharacters;

    /** Achievement list */
    UPROPERTY(BlueprintReadWrite, SaveGame)
    TSet<FString> UnlockedAchievements;

    /** Lifetime statistics */
    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 TotalRunsPlayed = 0;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 TotalRunsWon = 0;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 TotalMonstersKilled = 0;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 TotalElitesKilled = 0;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 TotalBossesKilled = 0;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 TotalGoldCollected = 0;

    FPlayerProgressData()
    {
        // Initialize with default character
        UnlockedCharacters.Add("Swordsman", 1);
    }
};

/**
 * Current run data that resets each run
 */
USTRUCT(BlueprintType)
struct YCR_API FCurrentRunData
{
    GENERATED_BODY()

    /** Current character being played */
    UPROPERTY(BlueprintReadWrite)
    FName SelectedCharacter = "Swordsman";

    /** Current map level */
    UPROPERTY(BlueprintReadWrite)
    int32 CurrentMapLevel = 1;

    /** Current character level in this run */
    UPROPERTY(BlueprintReadWrite)
    int32 CurrentLevel = 1;

    /** Time elapsed in current run */
    UPROPERTY(BlueprintReadWrite)
    float RunTime = 0.0f;

    /** Stats for current run */
    UPROPERTY(BlueprintReadWrite)
    int32 MonstersKilled = 0;

    UPROPERTY(BlueprintReadWrite)
    int32 ElitesKilled = 0;

    UPROPERTY(BlueprintReadWrite)
    int32 BossesKilled = 0;

    UPROPERTY(BlueprintReadWrite)
    int32 GoldCollected = 0;

    UPROPERTY(BlueprintReadWrite)
    int32 ExperienceCollected = 0;

    /** Reset all data for new run */
    void Reset()
    {
        CurrentMapLevel = 1;
        CurrentLevel = 1;
        RunTime = 0.0f;
        MonstersKilled = 0;
        ElitesKilled = 0;
        BossesKilled = 0;
        GoldCollected = 0;
        ExperienceCollected = 0;
    }
};