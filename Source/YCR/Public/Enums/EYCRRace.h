#pragma once

#include "CoreMinimal.h"
#include "EYCRRace.generated.h"

/**
 * Monster race types based on Ragnarok Online
 * Affects damage calculations and special interactions
 */
UENUM(BlueprintType)
enum class EYCRRace : uint8
{
	Formless     UMETA(DisplayName = "Formless"),      // Gelatinous creatures, spirits
	Undead       UMETA(DisplayName = "Undead"),        // Zombies, skeletons, ghosts
	Beast        UMETA(DisplayName = "Beast"),         // Animals, wild creatures
	Plant        UMETA(DisplayName = "Plant"),         // Flora-based monsters
	Insect       UMETA(DisplayName = "Insect"),        // Bugs, arachnids
	Fish         UMETA(DisplayName = "Fish"),          // Aquatic creatures
	Demon        UMETA(DisplayName = "Demon"),         // Demonic entities
	Humanoid     UMETA(DisplayName = "Humanoid"),      // Human-like creatures
	Angel        UMETA(DisplayName = "Angel"),         // Divine beings
	Dragon       UMETA(DisplayName = "Dragon")         // Draconic creatures
};