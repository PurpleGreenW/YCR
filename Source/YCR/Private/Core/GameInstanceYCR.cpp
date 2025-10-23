// D:\Unreal Projects\YCR\Source\YCR\Private\Core\GameInstanceYCR.cpp

#include "YCR/Public/Core/GameInstanceYCR.h"
#include "YCR/Public/Core/YCRSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/DataTable.h"

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

void UGameInstanceYCR::StartNewRun(const FString& SelectedCharacterClass)
{
    // Reset run data
    CurrentRunData = FCurrentRunData();
    CurrentRunData.SelectedCharacter = SelectedCharacterClass;
    CurrentRunData.RunStartTime = FDateTime::Now();
    
    // Initialize run with character
    UE_LOG(LogTemp, Log, TEXT("Starting new run with character: %s"), *SelectedCharacterClass);
}

void UGameInstanceYCR::EndCurrentRun(bool bVictory)
{
    // Calculate run statistics
    FTimespan RunDuration = FDateTime::Now() - CurrentRunData.RunStartTime;
    
    // Update player progress
    if (bVictory)
    {
        PlayerProgress.TotalRunsWon++;
        
        // Update highest level reached
        if (CurrentRunData.CurrentLevel > PlayerProgress.HighestLevelReached)
        {
            PlayerProgress.HighestLevelReached = CurrentRunData.CurrentLevel;
        }
    }
    
    PlayerProgress.TotalRunsPlayed++;
    PlayerProgress.TotalMonstersKilled += CurrentRunData.MonstersKilled;
    PlayerProgress.TotalElitesKilled += CurrentRunData.ElitesKilled;
    PlayerProgress.TotalBossesKilled += CurrentRunData.BossesKilled;
    PlayerProgress.TotalGoldCollected += CurrentRunData.GoldCollected;
    
    // Check achievements
    CheckAndUnlockAchievements();
    
    // Save progress
    SaveGameData();
    
    // Return to main menu
    UGameplayStatics::OpenLevel(this, "MainMenu");
}

void UGameInstanceYCR::CollectCard(const FName& CardName)
{
    if (PlayerProgress.CollectedCards.Contains(CardName))
    {
        PlayerProgress.CollectedCards[CardName]++;
    }
    else
    {
        PlayerProgress.CollectedCards.Add(CardName, 1);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Collected card: %s (Total: %d)"), 
        *CardName.ToString(), PlayerProgress.CollectedCards[CardName]);
    
    SaveGameData();
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

bool UGameInstanceYCR::SpendEssence(int32 Amount)
{
    if (PlayerProgress.TotalEssence >= Amount)
    {
        PlayerProgress.TotalEssence -= Amount;
        SaveGameData();
        return true;
    }
    return false;
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

int32 UGameInstanceYCR::GetCharacterAdvancementLevel(const FName& CharacterClass) const
{
    const int32* Level = PlayerProgress.UnlockedCharacters.Find(CharacterClass);
    return Level ? *Level : 0;
}

void UGameInstanceYCR::UnlockAchievement(const FString& AchievementID)
{
    if (!PlayerProgress.UnlockedAchievements.Contains(AchievementID))
    {
        PlayerProgress.UnlockedAchievements.Add(AchievementID);
        
        // Broadcast achievement unlock event
        OnAchievementUnlocked.Broadcast(AchievementID);
        
        UE_LOG(LogTemp, Log, TEXT("Achievement Unlocked: %s"), *AchievementID);
        SaveGameData();
    }
}

bool UGameInstanceYCR::IsAchievementUnlocked(const FString& AchievementID) const
{
    return PlayerProgress.UnlockedAchievements.Contains(AchievementID);
}

TArray<FString> UGameInstanceYCR::GetUnlockedAchievements() const
{
    return PlayerProgress.UnlockedAchievements.Array();
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

void UGameInstanceYCR::OnPostLoadMap(UWorld* World)
{
    if (!World)
    {
        return;
    }
    
    // Restore any necessary state after map load
    UE_LOG(LogTemp, Log, TEXT("Map loaded: %s"), *World->GetMapName());
}

void UGameInstanceYCR::CheckAndUnlockAchievements()
{
    // Beispiel Achievement Checks
    
    // "First Blood" - Kill 1 monster
    if (CurrentRunData.MonstersKilled >= 1 && !IsAchievementUnlocked("FirstBlood"))
    {
        UnlockAchievement("FirstBlood");
    }
    
    // "Monster Hunter" - Kill 100 monsters in one run
    if (CurrentRunData.MonstersKilled >= 100 && !IsAchievementUnlocked("MonsterHunter"))
    {
        UnlockAchievement("MonsterHunter");
    }
    
    // "Elite Slayer" - Kill 10 elites in one run
    if (CurrentRunData.ElitesKilled >= 10 && !IsAchievementUnlocked("EliteSlayer"))
    {
        UnlockAchievement("EliteSlayer");
    }
    
    // "Boss Killer" - Kill your first boss
    if (CurrentRunData.BossesKilled >= 1 && !IsAchievementUnlocked("BossKiller"))
    {
        UnlockAchievement("BossKiller");
    }
    
    // "Rich" - Collect 1000 gold in one run
    if (CurrentRunData.GoldCollected >= 1000 && !IsAchievementUnlocked("Rich"))
    {
        UnlockAchievement("Rich");
    }
    
    // "Speedrunner" - Complete a level in under 5 minutes
    FTimespan RunTime = FDateTime::Now() - CurrentRunData.RunStartTime;
    if (RunTime.GetTotalMinutes() < 5.0 && CurrentRunData.CurrentLevel > 1 && 
        !IsAchievementUnlocked("Speedrunner"))
    {
        UnlockAchievement("Speedrunner");
    }
    
    // Total progress achievements
    if (PlayerProgress.TotalMonstersKilled >= 1000 && !IsAchievementUnlocked("Exterminator"))
    {
        UnlockAchievement("Exterminator");
    }
    
    if (PlayerProgress.TotalRunsWon >= 10 && !IsAchievementUnlocked("Veteran"))
    {
        UnlockAchievement("Veteran");
    }
}

// Getter Methods for UI/Stats
int32 UGameInstanceYCR::GetTotalEssence() const
{
    return PlayerProgress.TotalEssence;
}

const FPlayerProgressData& UGameInstanceYCR::GetPlayerProgress() const
{
    return PlayerProgress;
}

const FCurrentRunData& UGameInstanceYCR::GetCurrentRunData() const
{
    return CurrentRunData;
}

void UGameInstanceYCR::UpdateRunStats(int32 MonstersKilled, int32 ElitesKilled, 
    int32 BossesKilled, int32 GoldCollected)
{
    CurrentRunData.MonstersKilled += MonstersKilled;
    CurrentRunData.ElitesKilled += ElitesKilled;
    CurrentRunData.BossesKilled += BossesKilled;
    CurrentRunData.GoldCollected += GoldCollected;
    
    // Broadcast currency change if needed
    OnCurrencyChanged.Broadcast(CurrentRunData.GoldCollected);
}

void UGameInstanceYCR::SetCurrentLevel(int32 Level)
{
    CurrentRunData.CurrentLevel = Level;
}