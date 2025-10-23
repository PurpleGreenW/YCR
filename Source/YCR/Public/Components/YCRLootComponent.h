#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/EYCRLootTypes.h"
#include "Enums/EYCRMonsterTypes.h"
#include "YCRLootComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExpGemDropped, const FYCRExpGemData&, GemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoinDropped, int32, CoinValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuffScrollDropped, const FYCRBuffScrollData&, BuffData);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class YCR_API UYCRLootComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UYCRLootComponent();

protected:
    virtual void BeginPlay() override;

public:
    // Monster rank determines drop multipliers
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
    EYCRMonsterType MonsterType = EYCRMonsterType::Normal;

    // Base values for this monster
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
    int32 BaseExpValue = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
    int32 BaseCoinValue = 1;

    // Drop chances
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot", meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float BuffScrollDropChance = 5.0f; // 5% chance

    // Available buff scrolls this monster can drop
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
    TArray<EYCRBuffType> AvailableBuffScrolls;

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Loot")
    FOnExpGemDropped OnExpGemDropped;

    UPROPERTY(BlueprintAssignable, Category = "Loot")
    FOnCoinDropped OnCoinDropped;

    UPROPERTY(BlueprintAssignable, Category = "Loot")
    FOnBuffScrollDropped OnBuffScrollDropped;

    // Main drop function
    UFUNCTION(BlueprintCallable, Category = "Loot")
    void DropLoot(const FVector& DropLocation);

    // Get calculated values
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Loot")
    int32 GetCalculatedExpValue() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Loot")
    int32 GetCalculatedCoinValue() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Loot")
    float GetOutGameGemDropChance() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Loot")
    EYCRGemColor GetGemColorForRank() const;

private:
    void DropExpGems(const FVector& Location);
    void DropCoins(const FVector& Location);
    void DropBuffScroll(const FVector& Location);

    float GetExpMultiplierForRank() const;
    float GetCoinMultiplierForRank() const;
};