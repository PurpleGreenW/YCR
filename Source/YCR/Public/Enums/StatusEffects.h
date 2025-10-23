// YCRStatusEffects.h
#pragma once

#include "CoreMinimal.h"
#include "YCRStatusEffects.generated.h"

/**
 * Status effects that can be applied to characters
 * Multiple effects can be active simultaneously
 */
UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EStatusEffect : uint8
{
	None        = 0         UMETA(DisplayName = "None"),
	Slow        = 1 << 0    UMETA(DisplayName = "Slow"),
	Frozen      = 1 << 1    UMETA(DisplayName = "Frozen"),
	Burning     = 1 << 2    UMETA(DisplayName = "Burning"),
	Bleeding    = 1 << 3    UMETA(DisplayName = "Bleeding"),
	Poisoned    = 1 << 4    UMETA(DisplayName = "Poisoned"),
	Blind       = 1 << 5    UMETA(DisplayName = "Blind"),
	Stunned     = 1 << 6    UMETA(DisplayName = "Stunned"),
	Silenced    = 1 << 7    UMETA(DisplayName = "Silenced")
};

ENUM_CLASS_FLAGS(EYCRStatusEffect);

/**
 * Data for a single status effect instance
 */
USTRUCT(BlueprintType)
struct FYCRStatusEffectData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EYCRStatusEffect EffectType = EYCRStatusEffect::None;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 5.0f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TickInterval = 1.0f;  // For DoT effects
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectValue = 0.0f;  // Damage per tick, slow %, etc.
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Instigator = nullptr;
    
	// Internal tracking
	float TimeRemaining = 0.0f;
	float TimeSinceLastTick = 0.0f;
};