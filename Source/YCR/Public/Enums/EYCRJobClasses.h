// Copyright YCR Project

#pragma once

#include "CoreMinimal.h"
#include "EYCRJobClasses.generated.h"

/**
 * Alle verfügbaren Job-Klassen
 */
UENUM(BlueprintType)
enum class EYCRJobClasses : uint8
{
	None            UMETA(DisplayName = "None"),
    
	// Basis Klassen
	Swordsman       UMETA(DisplayName = "Swordsman"),
	Mage            UMETA(DisplayName = "Mage"),
	Archer          UMETA(DisplayName = "Archer"),
	Thief           UMETA(DisplayName = "Thief"),
	Acolyte         UMETA(DisplayName = "Acolyte"),
	Merchant        UMETA(DisplayName = "Merchant"),
    
	// Advanced Swordsman
	Knight          UMETA(DisplayName = "Knight"),
	Crusader        UMETA(DisplayName = "Crusader"),
    
	// Advanced Mage
	Wizard          UMETA(DisplayName = "Wizard"),
	Sage            UMETA(DisplayName = "Sage"),
    
	// Advanced Archer
	Hunter          UMETA(DisplayName = "Hunter"),
	Bard            UMETA(DisplayName = "Bard"),
	Dancer          UMETA(DisplayName = "Dancer"),
    
	// Advanced Thief
	Assassin        UMETA(DisplayName = "Assassin"),
	Rogue           UMETA(DisplayName = "Rogue"),
    
	// Advanced Acolyte
	Priest          UMETA(DisplayName = "Priest"),
	Monk            UMETA(DisplayName = "Monk"),
    
	// Advanced Merchant
	Blacksmith      UMETA(DisplayName = "Blacksmith"),
	Alchemist       UMETA(DisplayName = "Alchemist"),
    
	// Goofy Classes
	Orc             UMETA(DisplayName = "Orc"),
	Skeleton        UMETA(DisplayName = "Skeleton"),
	Poring          UMETA(DisplayName = "Poring"),
	Goblin          UMETA(DisplayName = "Goblin"),
	Zombie          UMETA(DisplayName = "Zombie"),
	Munak           UMETA(DisplayName = "Munak"),
    
	MAX             UMETA(Hidden)
};