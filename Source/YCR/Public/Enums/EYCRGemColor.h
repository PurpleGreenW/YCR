#pragma once

#include "CoreMinimal.h"
#include "EYCRGemColor.generated.h"

/**
 * Gem rarity colors following standard loot tier colors
 */
UENUM(BlueprintType)
enum class EYCRGemColor : uint8
{
    Common      UMETA(DisplayName = "Common (White)"),
    Uncommon    UMETA(DisplayName = "Uncommon (Green)"),
    Rare        UMETA(DisplayName = "Rare (Blue)"),
    Epic        UMETA(DisplayName = "Epic (Purple)")
};