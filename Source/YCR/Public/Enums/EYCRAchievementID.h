#pragma once
#include "CoreMinimal.h"
#include "EYCRAchievementID.generated.h"

UENUM(BlueprintType)
enum class EYCRAchievementID : uint8
{
	None            UMETA(DisplayName = "None"),
	FirstBlood      UMETA(DisplayName = "First Blood"),
	BossKiller      UMETA(DisplayName = "Boss Killer"),
	GoldCollector   UMETA(DisplayName = "Rich"),
	// ... weitere Achievements
};