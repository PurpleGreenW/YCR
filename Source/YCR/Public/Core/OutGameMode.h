// D:\Unreal Projects\YCR\Source\YCR\Public\Core\OutGameMode.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OutGameMode.generated.h"

// Forward declarations
class UGameInstanceYCR;

/**
 * Game Mode for out-of-game areas (Main Menu, Character Select, etc.)
 * Handles meta progression, unlocks, and menu navigation
 */
UCLASS()
class YCR_API AOutGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AOutGameMode();

    // =====================================================
    // Core Functions
    // =====================================================
    
    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
    virtual void BeginPlay() override;

    // =====================================================
    // Menu Navigation
    // =====================================================
    
    /** Start a new run with selected character */
    UFUNCTION(BlueprintCallable, Category = "YCR|Menu")
    void StartNewRun(const FName& SelectedCharacter, int32 MapLevel = 1);
    
    /** Continue from last checkpoint */
    UFUNCTION(BlueprintCallable, Category = "YCR|Menu")
    void ContinueRun();
    
    /** Load character selection screen */
    UFUNCTION(BlueprintCallable, Category = "YCR|Menu")
    void OpenCharacterSelect();
    
    /** Load upgrade shop */
    UFUNCTION(BlueprintCallable, Category = "YCR|Menu")
    void OpenUpgradeShop();
    
    /** Load achievement screen */
    UFUNCTION(BlueprintCallable, Category = "YCR|Menu")
    void OpenAchievements();

    // =====================================================
    // Character Management
    // =====================================================
    
    /** Get list of all available characters */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "YCR|Characters")
    TArray<FName> GetAvailableCharacters() const;
    
    /** Check if character is unlocked */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "YCR|Characters")
    bool IsCharacterUnlocked(const FName& CharacterClass) const;
    
    /** Get character advancement level */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "YCR|Characters")
    int32 GetCharacterAdvancement(const FName& CharacterClass) const;

    // =====================================================
    // Meta Progression
    // =====================================================
    
    /** Purchase permanent upgrade */
    UFUNCTION(BlueprintCallable, Category = "YCR|Upgrades")
    bool PurchaseUpgrade(const FName& UpgradeID, int32 Cost);
    
    /** Unlock new character */
    UFUNCTION(BlueprintCallable, Category = "YCR|Upgrades")
    bool UnlockCharacter(const FName& CharacterClass, int32 EssenceCost);
    
    /** Upgrade character advancement */
    UFUNCTION(BlueprintCallable, Category = "YCR|Upgrades")
    bool AdvanceCharacter(const FName& CharacterClass);

    // =====================================================
    // Save/Load
    // =====================================================
    
    /** Reset all progress */
    UFUNCTION(BlueprintCallable, Category = "YCR|Save")
    void ResetProgress();
    
    /** Export save data */
    UFUNCTION(BlueprintCallable, Category = "YCR|Save")
    FString ExportSaveData();
    
    /** Import save data */
    UFUNCTION(BlueprintCallable, Category = "YCR|Save")
    bool ImportSaveData(const FString& SaveString);

protected:
    // =====================================================
    // Configuration
    // =====================================================
    
    /** Available character classes */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YCR|Config")
    TMap<FName, int32> CharacterUnlockCosts;
    
    /** Map names for level selection */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YCR|Config")
    TArray<FName> MapNames = {
        "VerdantPlains",
        "ScorchingSands", 
        "EnchantedForest",
        "CursedCaverns"
    };

private:
    /** Cached game instance reference */
    UPROPERTY()
    UGameInstanceYCR* CachedGameInstance;
    
    /** Initialize menu state from save data */
    void InitializeMenuState();
    
    /** Validate and prepare run parameters */
    bool ValidateRunParameters(const FName& Character, int32 MapLevel);
};