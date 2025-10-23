#pragma once

#include "CoreMinimal.h"
#include "EYCRItemRarity.generated.h"

/**
 * Item rarity tiers
 */
UENUM(BlueprintType)
enum class EYCRItemRarity : uint8
{
	Common      UMETA(DisplayName = "Common"),
	Uncommon    UMETA(DisplayName = "Uncommon"),
	Rare        UMETA(DisplayName = "Rare"),
	Epic        UMETA(DisplayName = "Epic"),
	Legendary   UMETA(DisplayName = "Legendary"),
	Mythic      UMETA(DisplayName = "Mythic"),
	Heroic      UMETA(DisplayName = "Heroic"),
	Glorious    UMETA(DisplayName = "Glorious"),
	Ultimate    UMETA(DisplayName = "Ultimate"),
	Godlike     UMETA(DisplayName = "Godlike")
};