#pragma once

#include "CoreMinimal.h"
#include "ECharacterState.generated.h"

/**
 * Character states for state machine
 */
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Idle        UMETA(DisplayName = "Idle"),
	Moving      UMETA(DisplayName = "Moving"),
	Attacking   UMETA(DisplayName = "Attacking"),
	Dead        UMETA(DisplayName = "Dead")
};