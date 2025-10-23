#include "Core/YCRGems.h"
#include "Enums/EYCRMonsterTypes.h"
#include "Enums/EYCRGemColor.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UYCRGems::UYCRGems()
{
    // Default values
    GemName = TEXT("Experience Gem");
    GemColor = EYCRGemColor::Common;
    ExperienceValue = 1;
    MeshScale = FVector(0.5f, 0.5f, 0.5f); // Gems are usually small
}

int32 UYCRGems::GetExperienceForMonsterType(EYCRMonsterType MonsterType)
{
    // Experience values based on monster difficulty
    switch (MonsterType)
    {
        case EYCRMonsterType::Normal:
            return 1;  // White gems
            
        case EYCRMonsterType::Elite:
            return 5;  // Green gems
            
        case EYCRMonsterType::MiniBoss:
            return 10; // Blue gems
            
        case EYCRMonsterType::Boss:
            return 25; // Purple gems
            
        case EYCRMonsterType::Special:
            return 50; // Red gems
            
        default:
            return 1;
    }
}

// Implementation for UYCRGemSpawner
AYCRGemPickup* UYCRGemSpawner::SpawnGem(
    UObject* WorldContextObject,
    TSubclassOf<AYCRGemPickup> GemClass,
    const FVector& Location,
    EYCRGemColor GemColor,
    int32 ExperienceValue)
{
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World || !GemClass)
    {
        return nullptr;
    }

    // Spawn parameters
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    // Spawn the gem
    AYCRGemPickup* SpawnedGem = World->SpawnActor<AYCRGemPickup>(GemClass, Location, FRotator::ZeroRotator, SpawnParams);
    
    if (SpawnedGem)
    {
        // Set gem properties
        SpawnedGem->SetGemProperties(GemColor, ExperienceValue);
    }

    return SpawnedGem;
}

void UYCRGemSpawner::SpawnGemsForMonster(
    UObject* WorldContextObject,
    TSubclassOf<AYCRGemPickup> GemClass,
    const FVector& Location,
    EYCRMonsterType MonsterType,
    int32 GemCount)
{
    if (!GemClass || GemCount <= 0)
    {
        return;
    }

    // Determine gem color based on monster type
    EYCRGemColor GemColor = EYCRGemColor::Common;
    switch (MonsterType)
    {
        case EYCRMonsterType::Normal:
            GemColor = EYCRGemColor::Common;
            break;
        case EYCRMonsterType::Elite:
            GemColor = EYCRGemColor::Uncommon;
            break;
        case EYCRMonsterType::MiniBoss:
            GemColor = EYCRGemColor::Rare;
            break;
        case EYCRMonsterType::Boss:
            GemColor = EYCRGemColor::Epic;
            break;
        case EYCRMonsterType::Special:
            GemColor = EYCRGemColor::Epic;
            break;
    }

    // Get experience value for this monster type
    int32 ExpValue = UYCRGems::GetExperienceForMonsterType(MonsterType);

    // Spawn multiple gems with slight position offset
    for (int32 i = 0; i < GemCount; i++)
    {
        // Add random offset to prevent gems spawning on top of each other
        FVector SpawnLocation = Location;
        SpawnLocation.X += FMath::RandRange(-50.0f, 50.0f);
        SpawnLocation.Y += FMath::RandRange(-50.0f, 50.0f);
        SpawnLocation.Z += 10.0f; // Slight elevation

        SpawnGem(WorldContextObject, GemClass, SpawnLocation, GemColor, ExpValue);
    }
}