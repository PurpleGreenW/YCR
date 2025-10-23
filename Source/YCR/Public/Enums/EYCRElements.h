#pragma once

#include "CoreMinimal.h"
#include "EYCRElements.generated.h"

/**
 * Element types for damage calculation
 */
UENUM(BlueprintType)
enum class EYCRElementType : uint8
{
	Neutral     UMETA(DisplayName = "Neutral"),
	Water       UMETA(DisplayName = "Water"),
	Earth       UMETA(DisplayName = "Earth"),
	Fire        UMETA(DisplayName = "Fire"),
	Wind        UMETA(DisplayName = "Wind"),
	Poison      UMETA(DisplayName = "Poison"),
	Holy        UMETA(DisplayName = "Holy"),
	Shadow      UMETA(DisplayName = "Shadow"),
	Ghost       UMETA(DisplayName = "Ghost"),
	Undead      UMETA(DisplayName = "Undead"),
    
	MAX         UMETA(Hidden)
};


/**
 * Element level for monsters (1-4)
 * Higher levels = stronger element affinity
 */
UENUM(BlueprintType)
enum class EYCRElementLevel : uint8
{
	Level1 UMETA(DisplayName = "Level 1"),     // 25% element
	Level2 UMETA(DisplayName = "Level 2"),     // 50% element
	Level3 UMETA(DisplayName = "Level 3"),     // 75% element
	Level4 UMETA(DisplayName = "Level 4"),     // 100% element
	MAX UMETA(Hidden)
};