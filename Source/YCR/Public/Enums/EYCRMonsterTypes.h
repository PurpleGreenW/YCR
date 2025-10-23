// EMonsterTypes.h
#pragma once

#include "CoreMinimal.h"
#include "EYCRMonsterTypes.generated.h"

/**
 * Monster sizes for scaling and gameplay mechanics
 */
UENUM(BlueprintType)
enum class EMonsterSize : uint8
{
	Small       UMETA(DisplayName = "Small"),      // 50% scale (Porings, Fabres)
	Medium      UMETA(DisplayName = "Medium"),     // 100% scale (Wolves, Lunatics)
	Large       UMETA(DisplayName = "Large"),      // 150% scale (Orcs, Bigfoot)
	Huge        UMETA(DisplayName = "Huge"),       // 200%+ scale (Bosses)
};

/**
 * Monster races for damage calculations and resistances
 */
UENUM(BlueprintType)
enum class EMonsterRace : uint8
{
	Formless    UMETA(DisplayName = "Formless"),   // Porings, Slimes
	Undead      UMETA(DisplayName = "Undead"),     // Zombies, Skeletons
	Brute       UMETA(DisplayName = "Brute"),      // Orcs, Goblins
	Plant       UMETA(DisplayName = "Plant"),      // Mandragora, Flora
	Insect      UMETA(DisplayName = "Insect"),     // Thief Bug, Hornet
	Fish        UMETA(DisplayName = "Fish"),       // Aquatic monsters
	Demon       UMETA(DisplayName = "Demon"),      // Deviruchi, Baphomet
	Humanoid    UMETA(DisplayName = "Humanoid"),   // Human-like enemies
	Angel       UMETA(DisplayName = "Angel"),      // Holy creatures
	Dragon      UMETA(DisplayName = "Dragon"),     // Dragon types
};

/**
 * Monster types/categories for spawn management
 */
UENUM(BlueprintType)
enum class EYCRMonsterTypes : uint8
{
	Normal      UMETA(DisplayName = "Normal"),
	Elite       UMETA(DisplayName = "Elite"),
	MiniBoss    UMETA(DisplayName = "Mini Boss"),
	Boss        UMETA(DisplayName = "Boss"),
	Special     UMETA(DisplayName = "Special"),     // Event monsters
};