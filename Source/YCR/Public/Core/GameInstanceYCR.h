// D:\Unreal Projects\YCR\Source\YCR\Public\Core\GameInstanceYCR.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "GameInstanceYCR.generated.h"

// Forward declarations
class UDataTable;
class USaveGame;
struct FStreamableHandle;

USTRUCT(BlueprintType)
struct FPlayerProgressData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 HighestMapUnlocked = 1;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 HighestLevelReached = 1;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 TotalEssence = 0;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 TotalGold = 0;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    TMap<FName, int32> UnlockedCharacters;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    TMap<FName, int32> CollectedCards;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    TMap<FName, bool> CompletedAchievements;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    TMap<FName, int32> MonsterKillCounts;
};

USTRUCT(BlueprintType)
struct FRunSessionData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FName SelectedCharacterClass = "Swordsman";

    UPROPERTY(BlueprintReadWrite)
    int32 CurrentMapLevel = 1;

    UPROPERTY(BlueprintReadWrite)
    float RunTime = 0.0f;

    UPROPERTY(BlueprintReadWrite)
    int32 MonstersKilled = 0;

    UPROPERTY(BlueprintReadWrite)
    int32 GoldCollected = 0;

    UPROPERTY(BlueprintReadWrite)
    TArray<FName> EquippedCards;

    void Reset()
    {
        CurrentMapLevel = 1;
        RunTime = 0.0f;
        MonstersKilled = 0;
        GoldCollected = 0;
        EquippedCards.Empty();
    }
};

/**
 * Main Game Instance for YCR
 * Handles persistent data, save games, and session management
 */
UCLASS()
class YCR_API UGameInstanceYCR : public UGameInstance
{
    GENERATED_BODY()

public:
    UGameInstanceYCR();

    // =====================================================
    // UGameInstance Interface
    // =====================================================
    virtual void Init() override;
    virtual void Shutdown() override;

    // =====================================================
    // Advanced Save/Load Features
    // =====================================================
    
    /** Get save game metadata without loading full save */
    UFUNCTION(BlueprintCallable, Category = "YCR|SaveGame")
    bool GetSaveGameMetadata(FDateTime& OutLastSaveTime, int32& OutSaveVersion);
    
    /** Export save to string for cloud saves */
    UFUNCTION(BlueprintCallable, Category = "YCR|SaveGame")
    FString ExportSaveToString();
    
    /** Import save from string */
    UFUNCTION(BlueprintCallable, Category = "YCR|SaveGame")
    bool ImportSaveFromString(const FString& SaveString);
    
    /** Auto-save functionality */
    UFUNCTION(BlueprintCallable, Category = "YCR|SaveGame")
    void EnableAutoSave(float IntervalInSeconds = 300.0f);
    
    UFUNCTION(BlueprintCallable, Category = "YCR|SaveGame")
    void DisableAutoSave();

    // =====================================================
    // Run Management
    // =====================================================
    
    UFUNCTION(BlueprintCallable, Category = "YCR|Run")
    void StartNewRun(const FName& CharacterClass, const TArray<FName>& SelectedCards);

    UFUNCTION(BlueprintCallable, Category = "YCR|Run")
    void EndCurrentRun(bool bVictory);

    UFUNCTION(BlueprintPure, Category = "YCR|Run")
    const FRunSessionData& GetCurrentRunData() const { return CurrentRunData; }

    UFUNCTION(BlueprintCallable, Category = "YCR|Run")
    void UpdateRunTime(float DeltaTime) { CurrentRunData.RunTime += DeltaTime; }

    // =====================================================
    // Progress Management
    // =====================================================
    
    UFUNCTION(BlueprintCallable, Category = "YCR|Progress")
    void UnlockMap(int32 MapLevel);

    UFUNCTION(BlueprintCallable, Category = "YCR|Progress")
    bool IsMapUnlocked(int32 MapLevel) const;

    UFUNCTION(BlueprintCallable, Category = "YCR|Progress")
    void UnlockCharacter(const FName& CharacterClass, int32 AdvancementLevel = 1);

    UFUNCTION(BlueprintCallable, Category = "YCR|Progress")
    bool IsCharacterUnlocked(const FName& CharacterClass) const;

    UFUNCTION(BlueprintCallable, Category = "YCR|Progress")
    void AddCard(const FName& CardName, int32 Count = 1);

    UFUNCTION(BlueprintPure, Category = "YCR|Progress")
    int32 GetCardCount(const FName& CardName) const;

    UFUNCTION(BlueprintCallable, Category = "YCR|Progress")
    void AddEssence(int32 Amount);

    UFUNCTION(BlueprintPure, Category = "YCR|Progress")
    int32 GetEssence() const { return PlayerProgress.TotalEssence; }

    // =====================================================
    // Achievement System
    // =====================================================
    
    UFUNCTION(BlueprintCallable, Category = "YCR|Achievement")
    void RecordMonsterKill(const FName& MonsterType);

    UFUNCTION(BlueprintCallable, Category = "YCR|Achievement")
    void CheckAndUnlockAchievements();

    UFUNCTION(BlueprintCallable, Category = "YCR|Achievement")
    void UnlockAchievement(const FName& AchievementID);

    UFUNCTION(BlueprintPure, Category = "YCR|Achievement")
    bool IsAchievementUnlocked(const FName& AchievementID) const;

    // =====================================================
    // Map Transition
    // =====================================================
    
    UFUNCTION(BlueprintCallable, Category = "YCR|Map")
    void TransitionToNextLevel();

    UFUNCTION(BlueprintCallable, Category = "YCR|Map")
    void TransitionToMap(const FName& MapName);

    // =====================================================
    // Data Tables
    // =====================================================
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YCR|Data")
    UDataTable* CharacterDataTable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YCR|Data")
    UDataTable* CardDataTable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YCR|Data")
    UDataTable* AchievementDataTable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YCR|Data")
    UDataTable* MonsterDataTable;

protected:
    // =====================================================
    // Internal Data
    // =====================================================
    
    UPROPERTY()
    FPlayerProgressData PlayerProgress;

    UPROPERTY()
    FRunSessionData CurrentRunData;

    UPROPERTY()
    FString SaveSlotName = TEXT("YCRSaveGame");

    UPROPERTY()
    int32 UserIndex = 0;

    /** Auto-save timer handle */
    FTimerHandle AutoSaveTimerHandle;
    
    /** Auto-save callback */
    void PerformAutoSave();

    // Map names for transitions
    TArray<FName> MapProgression = {
        "DevMap_01",  // For now, later: VerdantPlains_01 through 05
        "DevMap_02"   // For now, later: ScorchingSands_01 through 05
    };

private:
    // Helper functions
    void InitializeDefaultData();
    void OnPreLoadMap(const FString& MapName);
    void OnPostLoadMap(UWorld* LoadedWorld);
};