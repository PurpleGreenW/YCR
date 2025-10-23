// EMonsterTypes.h
#pragma once

#include "CoreMinimal.h"
#include "EYCRMonsterTypes.generated.h"



/**
 * Monster types/categories for spawn management
 */
UENUM(BlueprintType)
enum class EYCRMonsterType : uint8
{
	Normal      UMETA(DisplayName = "Normal"),
	Elite       UMETA(DisplayName = "Elite"),
	MiniBoss    UMETA(DisplayName = "Mini Boss"),
	Boss        UMETA(DisplayName = "Boss"),
	Special     UMETA(DisplayName = "Special"),     // Event monsters
};