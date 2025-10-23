// D:\Unreal Projects\YCR\Source\YCR\Public\Core\GameInstanceYCR.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "GameInstanceYCR.generated.h"

// Forward declarations
class UYCRSaveGame;

USTRUCT(BlueprintType)
struct FCurrentRunData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FString SelectedCharacter = "Swordsman";
    
    UPROPERTY(BlueprintReadWrite)
    int32 CurrentMapLevel = 1;
    
    UPROPERTY(BlueprintReadWrite)
    float RunTime = 0.0f;
    
    UPROPERTY(BlueprintReadWrite)
    int32 MonstersKilled = 0;
    
    UPROPERTY(BlueprintReadWrite)
    int32 ElitesKilled = 0;
    
    UPROPERTY(BlueprintReadWrite)
    int32 BossesKilled = 0;
    
    UPROPERTY(BlueprintReadWrite)
    int32 GoldCollected = 0;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<int32> EquippedCards;
    
    void Reset()
    {
        CurrentMapLevel = 1;
        RunTime = 0.0f;
        MonstersKilled = 0;
        GoldCollected = 0;
        EquippedCards.Empty();
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrencyChanged, int32, NewAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAchievementUnlocked, const FString&, AchievementID);

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
    
    // UGameInstance interface
    virtual void Init() override;
    virtual void Shutdown() override;
    
    // Save/Load functionality
    UFUNCTION(BlueprintCallable, Category = "Save Game")
    void SaveGameData();
    
    UFUNCTION(BlueprintCallable, Category = "Save Game")
    void LoadGameData();
    
    UFUNCTION(BlueprintCallable, Category = "Save Game")
    bool DoesSaveGameExist() const;
    
    UFUNCTION(BlueprintCallable, Category = "Save Game")
    void ResetProgress();
    
    // Current Run Management
    UFUNCTION(BlueprintCallable, Category = "Run")
    void StartNewRun(const FString& CharacterClass, const TArray<int32>& SelectedCards);
    
    UFUNCTION(BlueprintCallable, Category = "Run")
    void EndCurrentRun(bool bVictory);
    
    UFUNCTION(BlueprintPure, Category = "Run")
    FORCEINLINE FCurrentRunData GetCurrentRunData() const { return CurrentRunData; }
    
    // Currency Management
    UFUNCTION(BlueprintCallable, Category = "Currency")
    void AddEssence(int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Currency")
    void SpendEssence(int32 Amount);
    
    UFUNCTION(BlueprintPure, Category = "Currency")
    FORCEINLINE int32 GetEssence() const { return PlayerProgress.TotalEssence; }
    
    // Character Unlocks
    UFUNCTION(BlueprintCallable, Category = "Characters")
    bool IsCharacterUnlocked(const FString& CharacterName) const;
    
    UFUNCTION(BlueprintCallable, Category = "Characters")
    void UnlockCharacter(const FString& CharacterName);
    
    // Card System
    UFUNCTION(BlueprintCallable, Category = "Cards")
    void AddCard(int32 CardID);
    
    UFUNCTION(BlueprintCallable, Category = "Cards")
    bool HasCard(int32 CardID) const;
    
    // Achievement System
    UFUNCTION(BlueprintCallable, Category = "Achievements")
    void UnlockAchievement(const FString& AchievementID);
    
    UFUNCTION(BlueprintPure, Category = "Achievements")
    bool IsAchievementUnlocked(const FString& AchievementID) const;
    
    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnCurrencyChanged OnEssenceChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnAchievementUnlocked OnAchievementUnlocked;
    
protected:
    UPROPERTY()
    FPlayerProgressData PlayerProgress;
    
    UPROPERTY()
    FCurrentRunData CurrentRunData;
    
    UPROPERTY()
    UYCRSaveGame* CurrentSaveGame;
    
private:
    void InitializeDefaultData();
    void CheckAndUnlockAchievements();
    
    // Save game constants
    const FString SaveSlotName = TEXT("YCRSaveSlot");
    const int32 UserIndex = 0;
};