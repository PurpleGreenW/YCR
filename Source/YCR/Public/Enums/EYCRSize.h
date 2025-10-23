#pragma once

#include "CoreMinimal.h"
#include "EYCRSize.generated.h"

/**
 * Monster sizes for scaling and gameplay mechanics
 * Affects HP multiplier, damage, and hit box size
 */
UENUM(BlueprintType)
enum class EYCRSize : uint8
{
	Small UMETA(DisplayName = "Small"),         // 50% HP, smaller hitbox
	Medium UMETA(DisplayName = "Medium"),       // 100% HP, normal hitbox  
	Large UMETA(DisplayName = "Large"),         // 150% HP, 133% scale
	Huge UMETA(DisplayName = "Huge"),           // 200%+ HP, 200%+ scale
	MAX UMETA(Hidden)
};