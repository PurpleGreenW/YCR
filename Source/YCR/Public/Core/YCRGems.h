#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Enums/EYCRGemColor.h"
#include "YCRGems.generated.h"

/**
 * Data asset for gem definitions
 * Gems provide experience points when collected
 */
UCLASS(BlueprintType)
class YCR_API UYCRGems : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UYCRGems();

    // Gem Properties
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gem")
    FName GemName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gem")
    EYCRGemColor GemColor;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gem", meta = (ClampMin = "1"))
    int32 ExperienceValue = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gem")
    class UStaticMesh* GemMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gem")
    class UMaterialInterface* GemMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gem")
    FVector MeshScale = FVector(1.0f, 1.0f, 1.0f);

    // Optional effects
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
    class UParticleSystem* CollectionEffect;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
    class USoundBase* CollectionSound;

    // Helper function to get experience value based on monster type
    UFUNCTION(BlueprintPure, Category = "Gem")
    static int32 GetExperienceForMonsterType(EYCRMonsterType MonsterType);
};

/**
 * Gem spawning helper class
 */
UCLASS(BlueprintType)
class YCR_API UYCRGemSpawner : public UObject
{
    GENERATED_BODY()

public:
    // Spawn a gem at location
    UFUNCTION(BlueprintCallable, Category = "Gem", meta = (WorldContext = "WorldContextObject"))
    static class AYCRGemPickup* SpawnGem(
        UObject* WorldContextObject,
        TSubclassOf<class AYCRGemPickup> GemClass,
        const FVector& Location,
        EYCRGemColor GemColor,
        int32 ExperienceValue
    );

    // Spawn gem based on monster type
    UFUNCTION(BlueprintCallable, Category = "Gem", meta = (WorldContext = "WorldContextObject"))
    static void SpawnGemsForMonster(
        UObject* WorldContextObject,
        TSubclassOf<class AYCRGemPickup> GemClass,
        const FVector& Location,
        EYCRMonsterType MonsterType,
        int32 GemCount = 1
    );
};