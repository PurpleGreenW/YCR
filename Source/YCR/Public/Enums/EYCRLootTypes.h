#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EYCRLootTypes.generated.h"

UENUM(BlueprintType)
enum class EYCRGemType : uint8
{
    InGame       UMETA(DisplayName = "In-Game Gem"),
    OutGame      UMETA(DisplayName = "Out-Game Gem")
};

UENUM(BlueprintType)
enum class EYCRGemColor : uint8
{
    White        UMETA(DisplayName = "White (Normal)"),
    Green        UMETA(DisplayName = "Green (Elite)"),
    Blue         UMETA(DisplayName = "Blue (MiniBoss)"),
    Purple       UMETA(DisplayName = "Purple (Boss)")
};
/*
UENUM(BlueprintType)
enum class EYCRMonsterType : uint8
{
    Normal       UMETA(DisplayName = "Normal"),
    Elite        UMETA(DisplayName = "Elite"),
    MiniBoss     UMETA(DisplayName = "MiniBoss"),
    Boss         UMETA(DisplayName = "Boss")
};
*/
UENUM(BlueprintType)
enum class EYCRBuffType : uint8
{
    Frenzy          UMETA(DisplayName = "Frenzy (Attack Speed)"),
    Quick           UMETA(DisplayName = "Quick (Movement Speed)"),
    Shield          UMETA(DisplayName = "Shield (Temporary Shield)"),
    LifeSteal       UMETA(DisplayName = "Life Steal"),
    CritBoost       UMETA(DisplayName = "Critical Boost"),
    DamageBoost     UMETA(DisplayName = "Damage Boost"),
    Invulnerable    UMETA(DisplayName = "Invulnerability")
};

USTRUCT(BlueprintType)
struct FYCRExpGemData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EYCRGemType GemType = EYCRGemType::InGame;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EYCRGemColor GemColor = EYCRGemColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ExpValue = 1;

    FYCRExpGemData()
    {
        GemType = EYCRGemType::InGame;
        GemColor = EYCRGemColor::White;
        ExpValue = 1;
    }
};

USTRUCT(BlueprintType)
struct FYCRBuffScrollData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EYCRBuffType BuffType = EYCRBuffType::Frenzy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EffectMagnitude = 0.25f; // 25% increase

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description = "Temporary Buff";

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UTexture2D> Icon;

    FYCRBuffScrollData()
    {
        BuffType = EYCRBuffType::Frenzy;
        Duration = 5.0f;
        EffectMagnitude = 0.25f;
    }
};