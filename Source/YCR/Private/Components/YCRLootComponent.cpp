#include "Components/YCRLootComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Constructor
UYCRLootComponent::UYCRLootComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UYCRLootComponent::BeginPlay()
{
    Super::BeginPlay();
}

// Get coin amount based on monster type
int32 UYCRLootComponent::GetCoinDropAmount(EYCRMonsterType MonsterType) const
{
    switch (MonsterType)
    {
        case EYCRMonsterType::Normal:
            return 1;   // 1 coin
        case EYCRMonsterType::Elite:
            return 3;   // 3 coins
        case EYCRMonsterType::MiniBoss:
            return 10;  // 10 coins
        case EYCRMonsterType::Boss:
            return 100; // 100 coins
        case EYCRMonsterType::Special:
            return 100; // 100 coins
        default:
            return 1;   // Default to 1 coin
    }
}

// Calculate total coins with variance
int32 UYCRLootComponent::GetCalculatedCoinDrop(EYCRMonsterType MonsterType) const
{
    // Get base coin amount for monster type
    int32 BaseCoinAmount = GetCoinDropAmount(MonsterType);
    
    // Add some variance for non-normal monsters (±10%)
    if (MonsterType != EYCRMonsterType::Normal)
    {
        float Variance = FMath::RandRange(0.9f, 1.1f);
        BaseCoinAmount = FMath::RoundToInt(BaseCoinAmount * Variance);
    }
    
    return FMath::Max(1, BaseCoinAmount); // Ensure at least 1 coin drops
}

// Drop coins at monster death
void UYCRLootComponent::DropCoins(const FVector& DropLocation, EYCRMonsterType MonsterType)
{
    int32 CoinAmount = GetCalculatedCoinDrop(MonsterType);
    
    // Log the drop for debugging
    UE_LOG(LogTemp, Log, TEXT("Dropping %d coins for monster type: %s"), 
        CoinAmount, 
        *UEnum::GetValueAsString(MonsterType));
    
    // Trigger the coin drop event
    OnCoinDropped.Broadcast(CoinAmount, DropLocation);
    
    // TODO: Spawn actual coin pickup actors here
    // This would spawn coin pickup actors in the world
}

// Get experience multiplier for monster type
float UYCRLootComponent::GetExpMultiplierForType(EYCRMonsterType MonsterType) const
{
    switch (MonsterType)
    {
        case EYCRMonsterType::Normal:
            return 1.0f;
        case EYCRMonsterType::Elite:
            return 3.0f;
        case EYCRMonsterType::MiniBoss:
            return 10.0f;
        case EYCRMonsterType::Boss:
            return 50.0f;
        case EYCRMonsterType::Special:
            return 100.0f;
        default:
            return 1.0f;
    }
}

// Get gem drop chance for monster type
float UYCRLootComponent::GetGemDropChanceForType(EYCRMonsterType MonsterType) const
{
    switch (MonsterType)
    {
        case EYCRMonsterType::Normal:
            return 1.0f;    // 1% chance
        case EYCRMonsterType::Elite:
            return 5.0f;    // 5% chance
        case EYCRMonsterType::MiniBoss:
            return 25.0f;   // 25% chance
        case EYCRMonsterType::Boss:
            return 100.0f;  // 100% chance (always drops)
        case EYCRMonsterType::Special:
            return 100.0f;  // 100% chance (always drops)
        default:
            return 1.0f;
    }
}

// Get gem color based on monster type
EYCRGemColor UYCRLootComponent::GetGemColorForType(EYCRMonsterType MonsterType) const
{
    switch (MonsterType)
    {
        case EYCRMonsterType::Normal:
            return EYCRGemColor::White;
        case EYCRMonsterType::Elite:
            return EYCRGemColor::Green;
        case EYCRMonsterType::MiniBoss:
            return EYCRGemColor::Blue;
        case EYCRMonsterType::Boss:
            return EYCRGemColor::Purple;
        case EYCRMonsterType::Special:
            return EYCRGemColor::Red;   // Assuming Red for special
        default:
            return EYCRGemColor::White;
    }
}

// Main loot generation function
void UYCRLootComponent::GenerateLoot(const FVector& DropLocation, EYCRMonsterType MonsterType, int32 MonsterLevel)
{
    // Drop coins
    DropCoins(DropLocation, MonsterType);
    
    // Calculate experience
    int32 BaseExp = 10; // Base experience value
    float ExpMultiplier = GetExpMultiplierForType(MonsterType);
    int32 TotalExp = FMath::RoundToInt(BaseExp * ExpMultiplier * MonsterLevel);
    
    // Broadcast experience gained
    OnExperienceDropped.Broadcast(TotalExp);
    
    // Check for gem drops
    float GemChance = GetGemDropChanceForType(MonsterType);
    if (FMath::RandRange(0.0f, 100.0f) <= GemChance)
    {
        EYCRGemColor GemColor = GetGemColorForType(MonsterType);
        OnGemDropped.Broadcast(GemColor, DropLocation);
    }
    
    // TODO: Add additional loot types (items, equipment, etc.)
}