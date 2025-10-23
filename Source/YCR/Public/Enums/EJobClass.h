#pragma once

#include "CoreMinimal.h"
#include "EJobClass.generated.h"

/**
 * Player job classes
 */
UENUM(BlueprintType)
enum class EJobClass : uint8
{
	Novice		UMETA(DisplayName = "Novice"),
	Mage        UMETA(DisplayName = "Mage"),
	Swordsman   UMETA(DisplayName = "Swordsman"),
	Archer      UMETA(DisplayName = "Archer"),
	Thief       UMETA(DisplayName = "Thief"),
	Acolyte     UMETA(DisplayName = "Acolyte"),
	Merchant    UMETA(DisplayName = "Merchant")
};