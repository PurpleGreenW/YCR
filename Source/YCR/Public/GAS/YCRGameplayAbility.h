// Copyright YCR. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "YCR/Public/Enums/EElementTypes.h"
#include "YCRGameplayAbility.generated.h"

/**
 * Base class for all Gameplay Abilities in YCR
 * Extends UGameplayAbility with YCR-specific functionality
 */
UCLASS()
class YCR_API UYCRGameplayAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UYCRGameplayAbility();

    // =====================================
    // Ability Properties
    // =====================================
    
    /** Element type of this ability for damage calculations */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YCR|Ability")
    EElementTypes AbilityElement = EElementTypes::Neutral;
    
    /** Base damage multiplier for this ability */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YCR|Ability", meta = (ClampMin = "0.1", ClampMax = "10.0"))
    float BaseDamageMultiplier = 1.0f;
    
    /** Whether this ability can be upgraded through the rarity system */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YCR|Ability")
    bool bCanBeUpgraded = true;
    
    /** Current rarity level (1-10) if upgraded */
    UPROPERTY(BlueprintReadOnly, Category = "YCR|Ability")
    int32 CurrentRarityLevel = 1;

    // =====================================
    // Ability Functions
    // =====================================
    
    /** Called when ability is granted to ASC */
    virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
    
    /** Main ability activation */
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    
    /** Ability end/cleanup */
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

    // =====================================
    // Helper Functions
    // =====================================
    
    /** Calculate damage based on element matchup */
    UFUNCTION(BlueprintCallable, Category = "YCR|Ability")
    float CalculateElementalDamageMultiplier(EElementTypes TargetElement) const;
    
    /** Get rarity multiplier based on current rarity level */
    UFUNCTION(BlueprintCallable, Category = "YCR|Ability")
    float GetRarityMultiplier() const;
    
    /** Apply gameplay effect to target with YCR modifiers */
    UFUNCTION(BlueprintCallable, Category = "YCR|Ability")
    void ApplyYCRGameplayEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> EffectClass, float Level = 1.0f);

protected:
    /** Check if we have enough resources to activate */
    virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
        OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
    
    /** Apply cost after activation */
    virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
        const FGameplayAbilityActivationInfo ActivationInfo) const override;
};