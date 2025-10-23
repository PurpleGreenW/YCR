// D:\Unreal Projects\YCR\Source\YCR\Private\Core\GameInstanceYCR.cpp

#include "YCR/Public/Core/GameInstanceYCR.h"
#include "YCR/Public/Core/YCRSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/DataTable.h"

// Save game class


UGameInstanceYCR::UGameInstanceYCR()
{
    // Set default values
}

void UGameInstanceYCR::Init()
{
    Super::Init();

    // Initialize default data
    InitializeDefaultData();

    // Try to load existing save
    LoadGameData();

    // Bind to map load events
    FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UGameInstanceYCR::OnPreLoadMap);
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UGameInstanceYCR::OnPostLoadMap);
}

void UGameInstanceYCR::Shutdown()
{
    // Auto-save on shutdown
    SaveGameData();
    
    Super::Shutdown();
}

void UGameInstanceYCR::InitializeDefaultData()
{
    // Unlock default character
    PlayerProgress.UnlockedCharacters.Add("Swordsman", 1);
    
    // Initialize default values
    PlayerProgress.HighestMapUnlocked = 1;
    PlayerProgress.HighestLevelReached = 1;
}

void UGameInstanceYCR::SaveGameData()
{
    UYCRSaveGame* SaveGameInstance = Cast<UYCRSaveGame>(
        UGameplayStatics::CreateSaveGameObject(UYCRSaveGame::StaticClass())
    );

    if (SaveGameInstance)
    {
        SaveGameInstance->SavedProgress = PlayerProgress;
        SaveGameInstance->LastSaveTime = FDateTime::Now();
        
        if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, UserIndex))
        {
            UE_LOG(LogTemp, Log, TEXT("Game progress saved successfully at %s"), 
                *SaveGameInstance->LastSaveTime.ToString());
        }
    }
}

void UGameInstanceYCR::LoadGameData()
{
    if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex))
    {
        UYCRSaveGame* LoadedGame = Cast<UYCRSaveGame>(
            UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex)
        );

        if (LoadedGame)
        {
            // Version check für zukünftige Kompatibilität
            if (LoadedGame->SaveVersion == 1)
            {
                PlayerProgress = LoadedGame->SavedProgress;
                UE_LOG(LogTemp, Log, TEXT("Game progress loaded successfully from %s"), 
                    *LoadedGame->LastSaveTime.ToString());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Save file version mismatch. Expected: 1, Got: %d"), 
                    LoadedGame->SaveVersion);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("No save game found, using default values"));
        InitializeDefaultData();
    }
}

bool UGameInstanceYCR::DoesSaveGameExist() const
{
    return UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex);
}

void UGameInstanceYCR::ResetProgress()
{
    PlayerProgress = FPlayerProgressData();
    InitializeDefaultData();
    SaveGameData();
}

void UGameInstanceYCR::StartNewRun(const FName& CharacterClass, const TArray<FName>& SelectedCards)
{
    CurrentRunData.Reset();
    CurrentRunData.SelectedCharacterClass = CharacterClass;
    CurrentRunData.EquippedCards = SelectedCards;
    
    // Transition to first map
    TransitionToMap(MapProgression[0]);
}

void UGameInstanceYCR::EndCurrentRun(bool bVictory)
{
    // Update progress based on run results
    PlayerProgress.TotalGold += CurrentRunData.GoldCollected;
    
    if (bVictory)
    {
        // Check if we unlocked next level
        if (CurrentRunData.CurrentMapLevel >= PlayerProgress.HighestLevelReached)
        {
            PlayerProgress.HighestLevelReached = CurrentRunData.CurrentMapLevel + 1;
        }
    }
    
    // Check achievements
    CheckAndUnlockAchievements();
    
    // Save progress
    SaveGameData();
    
    // Return to main menu
    UGameplayStatics::OpenLevel(this, "MainMenu");
}

void UGameInstanceYCR::UnlockMap(int32 MapLevel)
{
    if (MapLevel > PlayerProgress.HighestMapUnlocked)
    {
        PlayerProgress.HighestMapUnlocked = MapLevel;
        SaveGameData();
    }
}

bool UGameInstanceYCR::IsMapUnlocked(int32 MapLevel) const
{
    return MapLevel <= PlayerProgress.HighestMapUnlocked;
}

void UGameInstanceYCR::UnlockCharacter(const FName& CharacterClass, int32 AdvancementLevel)
{
    if (PlayerProgress.UnlockedCharacters.Contains(CharacterClass))
    {
        PlayerProgress.UnlockedCharacters[CharacterClass] = 
            FMath::Max(PlayerProgress.UnlockedCharacters[CharacterClass], AdvancementLevel);
    }
    else
    {
        PlayerProgress.UnlockedCharacters.Add(CharacterClass, AdvancementLevel);
    }
    SaveGameData();
}

bool UGameInstanceYCR::IsCharacterUnlocked(const FName& CharacterClass) const
{
    return PlayerProgress.UnlockedCharacters.Contains(CharacterClass);
}

void UGameInstanceYCR::AddCard(const FName& CardName, int32 Count)
{
    if (PlayerProgress.CollectedCards.Contains(CardName))
    {
        PlayerProgress.CollectedCards[CardName] += Count;
    }
    else
    {
        PlayerProgress.CollectedCards.Add(CardName, Count);
    }
}

int32 UGameInstanceYCR::GetCardCount(const FName& CardName) const
{
    const int32* Count = PlayerProgress.CollectedCards.Find(CardName);
    return Count ? *Count : 0;
}

void UGameInstanceYCR::AddEssence(int32 Amount)
{
    PlayerProgress.TotalEssence += Amount;
    SaveGameData();
}

void UGameInstanceYCR::RecordMonsterKill(const FName& MonsterType)
{
    CurrentRunData.MonstersKilled++;
    
    if (PlayerProgress.MonsterKillCounts.Contains(MonsterType))
    {
        PlayerProgress.MonsterKillCounts[MonsterType]++;
    }
    else
    {
        PlayerProgress.MonsterKillCounts.Add(MonsterType, 1);
    }
}

void UGameInstanceYCR::CheckAndUnlockAchievements()
{
    // Beispiel Achievement Checks
    
    // "First Blood" - Kill 1 monster
    if (CurrentRunData.MonstersKilled >= 1 && !IsAchievementUnlocked("FirstBlood"))
    {
        UnlockAchievement("FirstBlood");
    }
    
    // "Poring Slayer" - Kill 100 Porings
    const int32* PoringKills = PlayerProgress.MonsterKillCounts.Find("Poring");
    if (PoringKills && *PoringKills >= 100 && !IsAchievementUnlocked("PoringSlayer"))
    {
        UnlockAchievement("PoringSlayer");
        AddEssence(50); // Reward
    }
    
    // "Speed Runner" - Complete level in under 5 minutes
    if (CurrentRunData.RunTime < 300.0f && !IsAchievementUnlocked("SpeedRunner"))
    {
        UnlockAchievement("SpeedRunner");
        AddEssence(100);
    }
}

void UGameInstanceYCR::UnlockAchievement(const FName& AchievementID)
{
    PlayerProgress.CompletedAchievements.Add(AchievementID, true);
    
    // TODO: Show achievement popup
    UE_LOG(LogTemp, Log, TEXT("Achievement Unlocked: %s"), *AchievementID.ToString());
}

bool UGameInstanceYCR::IsAchievementUnlocked(const FName& AchievementID) const
{
    const bool* IsUnlocked = PlayerProgress.CompletedAchievements.Find(AchievementID);
    return IsUnlocked && *IsUnlocked;
}

void UGameInstanceYCR::TransitionToNextLevel()
{
    CurrentRunData.CurrentMapLevel++;
    
    // For now, just alternate between DevMap_01 and DevMap_02
    int32 MapIndex = (CurrentRunData.CurrentMapLevel - 1) % MapProgression.Num();
    TransitionToMap(MapProgression[MapIndex]);
}

void UGameInstanceYCR::TransitionToMap(const FName& MapName)
{
    UGameplayStatics::OpenLevel(this, MapName);
}

void UGameInstanceYCR::OnPreLoadMap(const FString& MapName)
{
    // Save current run state before transition
    UE_LOG(LogTemp, Log, TEXT("Transitioning to map: %s"), *MapName);
}

void UGameInstanceYCR::OnPostLoadMap(UWorld* LoadedWorld)
{
    // Restore run state after transition
    if (LoadedWorld)
    {
        UE_LOG(LogTemp, Log, TEXT("Map loaded: %s"), *LoadedWorld->GetMapName());
    }
}