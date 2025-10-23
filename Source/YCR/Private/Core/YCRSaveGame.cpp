// YCRSaveGame.cpp

#include "YCR/Public/Core/YCRSaveGame.h"

UYCRSaveGame::UYCRSaveGame()
{
    SaveSlotName = TEXT("YCRSaveSlot1");
    UserIndex = 0;

    // Initialize with default unlocked character (Swordsman)
    FCharacterUnlockData DefaultCharacter;
    DefaultCharacter.CharacterClass = "Swordsman";
    DefaultCharacter.AdvancementLevel = 1;
    DefaultCharacter.bIsUnlocked = true;
    UnlockedCharacters.Add(DefaultCharacter);
}

bool UYCRSaveGame::IsCharacterUnlocked(const FName& CharacterClass) const
{
    for (const FCharacterUnlockData& CharData : UnlockedCharacters)
    {
        if (CharData.CharacterClass == CharacterClass && CharData.bIsUnlocked)
        {
            return true;
        }
    }
    return false;
}

int32 UYCRSaveGame::GetCharacterAdvancementLevel(const FName& CharacterClass) const
{
    for (const FCharacterUnlockData& CharData : UnlockedCharacters)
    {
        if (CharData.CharacterClass == CharacterClass)
        {
            return CharData.AdvancementLevel;
        }
    }
    return 0;
}

int32 UYCRSaveGame::GetCardCount(const FName& CardID) const
{
    for (const FCardSaveData& CardData : CollectedCards)
    {
        if (CardData.CardID == CardID)
        {
            return CardData.Count;
        }
    }
    return 0;
}

bool UYCRSaveGame::IsAchievementCompleted(const FName& AchievementID) const
{
    for (const FAchievementSaveData& Achievement : CompletedAchievements)
    {
        if (Achievement.AchievementID == AchievementID)
        {
            return Achievement.bIsCompleted;
        }
    }
    return false;
}

float UYCRSaveGame::GetCompletionPercentage() const
{
    float TotalProgress = 0.0f;
    
    // Characters unlocked (20%)
    const float CharacterWeight = 20.0f;
    const int32 TotalCharacters = 6; // Base classes
    int32 UnlockedCount = 0;
    for (const FCharacterUnlockData& CharData : UnlockedCharacters)
    {
        if (CharData.bIsUnlocked)
        {
            UnlockedCount++;
        }
    }
    TotalProgress += (CharacterWeight * UnlockedCount / TotalCharacters);
    
    // Maps completed (40%)
    const float MapWeight = 40.0f;
    const int32 TotalMaps = 20; // 4 regions x 5 levels
    int32 CompletedMaps = 0;
    for (const auto& MapPair : MapProgress)
    {
        CompletedMaps += FMath::Min(MapPair.Value.HighestLevelCompleted, 5);
    }
    TotalProgress += (MapWeight * CompletedMaps / TotalMaps);
    
    // Achievements (30%)
    const float AchievementWeight = 30.0f;
    const int32 TotalAchievements = 100; // Annahme
    TotalProgress += (AchievementWeight * CompletedAchievements.Num() / TotalAchievements);
    
    // Cards collected (10%)
    const float CardWeight = 10.0f;
    const int32 TotalCards = 50; // Annahme
    TotalProgress += (CardWeight * CollectedCards.Num() / TotalCards);
    
    return FMath::Clamp(TotalProgress, 0.0f, 100.0f);
}

void UYCRSaveGame::UpdatePlayStatistics(float SessionTime, const FName& PlayedCharacter)
{
    TotalPlayTime += SessionTime;
    TotalRuns++;
    
    // Update character play count
    if (CharacterPlayCounts.Contains(PlayedCharacter))
    {
        CharacterPlayCounts[PlayedCharacter]++;
    }
    else
    {
        CharacterPlayCounts.Add(PlayedCharacter, 1);
    }
    
    // Update most played character
    int32 HighestPlayCount = 0;
    for (const auto& CharPair : CharacterPlayCounts)
    {
        if (CharPair.Value > HighestPlayCount)
        {
            HighestPlayCount = CharPair.Value;
            MostPlayedCharacter = CharPair.Key;
        }
    }
    
    LastSaveTime = FDateTime::Now();
}