#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Enums/EYCRCharacterClasses.h"
#include "Enums/EYCRAchievementID.h"
#include "YCRSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FYCRSavedCharacterData
{
    GENERATED_BODY()

    UPROPERTY(SaveGame)
    FString PlayerName = "Novice";

    UPROPERTY(SaveGame)
    FName SelectedHairstyleID = "Default";

    UPROPERTY(SaveGame)
    EYCRCharacterClasses CharacterClass = EYCRCharacterClasses::Novice;

    UPROPERTY(SaveGame)
    int32 TotalRunsCompleted = 0;

    UPROPERTY(SaveGame)
    int32 HighestLevelReached = 0;
};

UCLASS()
class YCR_API UYCRSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UYCRSaveGame();

    // Current Character Data
    UPROPERTY(SaveGame, BlueprintReadOnly)
    FYCRSavedCharacterData CurrentCharacter;

    // Meta Progression
    UPROPERTY(SaveGame, BlueprintReadOnly)
    int32 TotalEssence = 0;

    UPROPERTY(SaveGame, BlueprintReadOnly)
    TArray<EYCRAchievementID> UnlockedAchievements;

    // Unlocked Content
    UPROPERTY(SaveGame, BlueprintReadOnly)
    TArray<EYCRCharacterClasses> UnlockedClasses;

    UPROPERTY(SaveGame, BlueprintReadOnly)
    TArray<FName> UnlockedHairstyles;

    // Helper Functions
    UFUNCTION(BlueprintCallable, Category = "Save Game")
    bool IsAchievementUnlocked(EYCRAchievementID AchievementID) const;

    UFUNCTION(BlueprintCallable, Category = "Save Game")
    void UnlockAchievement(EYCRAchievementID AchievementID);
};