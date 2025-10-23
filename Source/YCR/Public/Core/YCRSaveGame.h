// D:\Unreal Projects\YCR\Source\YCR\Public\Core\YCRSaveGame.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Engine/DataTable.h"
#include "YCRSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FCharacterProgressData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 TotalRunsCompleted = 0;
    
    UPROPERTY(BlueprintReadWrite)
    int32 HighestLevelReached = 1;
    
    UPROPERTY(BlueprintReadWrite)
    int32 TotalKills = 0;
    
    UPROPERTY(BlueprintReadWrite)
    int32 BossKills = 0;
    
    UPROPERTY(BlueprintReadWrite)
    float BestRunTime = 0.0f;
    
    UPROPERTY(BlueprintReadWrite)
    bool bIsUnlocked = false;
};

USTRUCT(BlueprintType)
struct FCardCollectionData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    int32 CardID = 0;
    
    UPROPERTY(BlueprintReadWrite)
    int32 CollectionCount = 0;
    
    UPROPERTY(BlueprintReadWrite)
    bool bIsEquipped = false;
};

/**
 * Save Game class for YCR
 */
UCLASS()
class YCR_API UYCRSaveGame : public USaveGame
{
    GENERATED_BODY()
    
public:
    UYCRSaveGame();
    
    // Save version for future compatibility
    UPROPERTY(VisibleAnywhere, Category = "Save Data")
    int32 SaveVersion;
    
    // Player Progress
    UPROPERTY(VisibleAnywhere, Category = "Progress")
    FPlayerProgressData SavedProgress;
    
    // Character-specific progress
    UPROPERTY(VisibleAnywhere, Category = "Characters")
    TMap<FString, FCharacterProgressData> CharacterProgress;
    
    // Card Collection
    UPROPERTY(VisibleAnywhere, Category = "Cards")
    TArray<FCardCollectionData> CollectedCards;
    
    // Meta Currencies
    UPROPERTY(VisibleAnywhere, Category = "Currency")
    int32 TotalEssence = 0;
    
    UPROPERTY(VisibleAnywhere, Category = "Currency")
    int32 TotalSilverCoins = 0;
    
    // Permanent Upgrades purchased
    UPROPERTY(VisibleAnywhere, Category = "Upgrades")
    TMap<FString, int32> PermanentUpgrades;
    
    // Settings
    UPROPERTY(VisibleAnywhere, Category = "Settings")
    float MasterVolume = 1.0f;
    
    UPROPERTY(VisibleAnywhere, Category = "Settings")
    float MusicVolume = 0.7f;
    
    UPROPERTY(VisibleAnywhere, Category = "Settings")
    float SFXVolume = 1.0f;
    
    // Statistics
    UPROPERTY(VisibleAnywhere, Category = "Stats")
    int32 TotalPlayTime = 0;
    
    UPROPERTY(VisibleAnywhere, Category = "Stats")
    FDateTime LastSaveTime;
    
    // Achievement IDs that have been unlocked
    UPROPERTY(VisibleAnywhere, Category = "Achievements")
    TArray<FString> UnlockedAchievements;
};