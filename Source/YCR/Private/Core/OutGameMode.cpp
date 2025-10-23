// D:\Unreal Projects\YCR\Source\YCR\Private\Core\OutGameMode.cpp

#include "YCR/Public/Core/OutGameMode.h"
#include "YCR/Public/Core/OutGameState.h"
#include "YCR/Public/Core/GameInstanceYCR.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AOutGameMode::AOutGameMode()
{
    // Set default game state
    GameStateClass = AOutGameState::StaticClass();
    
    // No default pawn in menus
    DefaultPawnClass = nullptr;
    
    // Initialize character costs
    CharacterUnlockCosts.Add("Swordsman", 0);      // Free starter
    CharacterUnlockCosts.Add("Mage", 100);
    CharacterUnlockCosts.Add("Archer", 150);
    CharacterUnlockCosts.Add("Thief", 200);
    CharacterUnlockCosts.Add("Acolyte", 250);
    CharacterUnlockCosts.Add("Merchant", 300);
    CharacterUnlockCosts.Add("Orc", 500);          // Monster characters
    CharacterUnlockCosts.Add("Skeleton", 500);
    CharacterUnlockCosts.Add("Poring", 1000);      // Meme character
}

void AOutGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);
    
    UE_LOG(LogTemp, Log, TEXT("YCR OutGameMode initialized for menu: %s"), *MapName);
}

void AOutGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // Cache game instance
    CachedGameInstance = Cast<UGameInstanceYCR>(GetGameInstance());
    
    // Initialize menu state
    InitializeMenuState();
}

void AOutGameMode::StartNewRun(const FName& SelectedCharacter, int32 MapLevel)
{
    if (!ValidateRunParameters(SelectedCharacter, MapLevel))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid run parameters"));
        return;
    }
    
    if (CachedGameInstance)
    {
        // Set run parameters
        CachedGameInstance->SetSelectedCharacter(SelectedCharacter);
        CachedGameInstance->SetCurrentLevel(MapLevel);
        
        // Determine map name based on level
        int32 MapIndex = (MapLevel - 1) / 5;  // 5 levels per map
        int32 SubLevel = ((MapLevel - 1) % 5) + 1;  // 1-5 within each map
        
        if (MapNames.IsValidIndex(MapIndex))
        {
            FString FullMapName = FString::Printf(TEXT("%s_L%d"), 
                *MapNames[MapIndex].ToString(), SubLevel);
            
            UE_LOG(LogTemp, Log, TEXT("Starting run: Character=%s, Map=%s"), 
                *SelectedCharacter.ToString(), *FullMapName);
            
            // Transition to gameplay map
            UGameplayStatics::OpenLevel(this, *FullMapName);
        }
    }
}

void AOutGameMode::ContinueRun()
{
    // For now, just start from last unlocked level
    if (CachedGameInstance)
    {
        int32 HighestUnlocked = CachedGameInstance->GetPlayerProgress().HighestMapUnlocked;
        FName LastCharacter = "Swordsman"; // TODO: Store last used character
        
        StartNewRun(LastCharacter, HighestUnlocked);
    }
}

void AOutGameMode::OpenCharacterSelect()
{
    // This would typically open a UI widget
    UE_LOG(LogTemp, Log, TEXT("Opening character selection screen"));
}

void AOutGameMode::OpenUpgradeShop()
{
    // This would typically open a UI widget
    UE_LOG(LogTemp, Log, TEXT("Opening upgrade shop"));
}

void AOutGameMode::OpenAchievements()
{
    // This would typically open a UI widget
    UE_LOG(LogTemp, Log, TEXT("Opening achievements screen"));
}

TArray<FName> AOutGameMode::GetAvailableCharacters() const
{
    TArray<FName> Characters;
    CharacterUnlockCosts.GetKeys(Characters);
    return Characters;
}

bool AOutGameMode::IsCharacterUnlocked(const FName& CharacterClass) const
{
    if (CachedGameInstance)
    {
        return CachedGameInstance->IsCharacterUnlocked(CharacterClass);
    }
    return false;
}

int32 AOutGameMode::GetCharacterAdvancement(const FName& CharacterClass) const
{
    if (CachedGameInstance)
    {
        return CachedGameInstance->GetCharacterAdvancementLevel(CharacterClass);
    }
    return 0;
}

bool AOutGameMode::PurchaseUpgrade(const FName& UpgradeID, int32 Cost)
{
    if (!CachedGameInstance)
    {
        return false;
    }
    
    if (CachedGameInstance->SpendEssence(Cost))
    {
        // TODO: Apply upgrade based on UpgradeID
        UE_LOG(LogTemp, Log, TEXT("Purchased upgrade: %s for %d essence"), 
            *UpgradeID.ToString(), Cost);
        return true;
    }
    
    return false;
}

bool AOutGameMode::UnlockCharacter(const FName& CharacterClass, int32 EssenceCost)
{
    if (!CachedGameInstance || IsCharacterUnlocked(CharacterClass))
    {
        return false;
    }
    
    if (CachedGameInstance->SpendEssence(EssenceCost))
    {
        CachedGameInstance->UnlockCharacter(CharacterClass, 1);
        UE_LOG(LogTemp, Log, TEXT("Unlocked character: %s"), *CharacterClass.ToString());
        return true;
    }
    
    return false;
}

bool AOutGameMode::AdvanceCharacter(const FName& CharacterClass)
{
    if (!CachedGameInstance || !IsCharacterUnlocked(CharacterClass))
    {
        return false;
    }
    
    int32 CurrentLevel = GetCharacterAdvancement(CharacterClass);
    int32 Cost = (CurrentLevel + 1) * 500; // Scaling cost
    
    if (CachedGameInstance->SpendEssence(Cost))
    {
        CachedGameInstance->UnlockCharacter(CharacterClass, CurrentLevel + 1);
        UE_LOG(LogTemp, Log, TEXT("Advanced character %s to level %d"), 
            *CharacterClass.ToString(), CurrentLevel + 1);
        return true;
    }
    
    return false;
}

void AOutGameMode::ResetProgress()
{
    if (CachedGameInstance)
    {
        CachedGameInstance->ResetProgress();
        InitializeMenuState();
    }
}

FString AOutGameMode::ExportSaveData()
{
    // TODO: Implement save export
    return TEXT("");
}

bool AOutGameMode::ImportSaveData(const FString& SaveString)
{
    // TODO: Implement save import
    return false;
}

void AOutGameMode::InitializeMenuState()
{
    if (!CachedGameInstance)
    {
        return;
    }
    
    // Ensure save data is loaded
    if (!CachedGameInstance->DoesSaveGameExist())
    {
        UE_LOG(LogTemp, Log, TEXT("No save file found, creating default save"));
        CachedGameInstance->SaveGameData();
    }
}

bool AOutGameMode::ValidateRunParameters(const FName& Character, int32 MapLevel)
{
    // Check if character is unlocked
    if (!IsCharacterUnlocked(Character))
    {
        UE_LOG(LogTemp, Warning, TEXT("Character %s is not unlocked"), *Character.ToString());
        return false;
    }
    
    // Check if map level is unlocked
    if (!CachedGameInstance || !CachedGameInstance->IsMapUnlocked(MapLevel))
    {
        UE_LOG(LogTemp, Warning, TEXT("Map level %d is not unlocked"), MapLevel);
        return false;
    }
    
    return true;
}