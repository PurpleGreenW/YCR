#pragma once

#include "CoreMinimal.h"
#include "EYCRElementTypes.generated.h"

/**
 * Element types for damage calculation
 */
UENUM(BlueprintType)
enum class EYCRElementTypes : uint8
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