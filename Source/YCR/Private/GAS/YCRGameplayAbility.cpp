// Copyright YCR. All Rights Reserved.

#include "GAS/YCRGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "YCR/Public/Character/CharacterBase.h"
#include "YCR/Public/GAS/YCRAttributeSet.h"

UYCRGameplayAbility::UYCRGameplayAbility()
{
    // Default ability settings
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
    NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
}

void UYCRGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnAvatarSet(ActorInfo, Spec);
    
    // Extract rarity level from ability spec if it exists
    if (Spec.Level > 0)
    {
        CurrentRarityLevel = FMath::Clamp(Spec.Level, 1, 10);
    }
}

void UYCRGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }
    
    // Base activation - derived classes implement specific logic
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UYCRGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    // Cleanup
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

float UYCRGameplayAbility::CalculateElementalDamageMultiplier(EYCRElementTypes TargetElement) const
{
    // Element effectiveness based on your design:
    // Fire > Wind > Earth > Lightning > Water > Fire
    // Holy <-> Shadow (mutual weakness)
    // Ghost <-> Physical
    
    float Multiplier = 1.0f;
    
    // Simplified element matchup - you can expand this based on your full element system
    switch (AbilityElement)
    {
    case EYCRElementTypes::Fire:
        if (TargetElement == EYCRElementTypes::Wind) Multiplier = 1.5f;
        else if (TargetElement == EYCRElementTypes::Water) Multiplier = 0.5f;
        break;
        
    case EYCRElementTypes::Water:
        if (TargetElement == EYCRElementTypes::Fire) Multiplier = 1.5f;
        else if (TargetElement == EYCRElementTypes::Wind) Multiplier = 0.5f;
        break;
        
    case EYCRElementTypes::Holy:
        if (TargetElement == EYCRElementTypes::Shadow) Multiplier = 2.0f;
        else if (TargetElement == EYCRElementTypes::Undead) Multiplier = 1.5f;
        break;
        
    case EYCRElementTypes::Shadow:
        if (TargetElement == EYCRElementTypes::Holy) Multiplier = 2.0f;
        break;
        
    // Add more element matchups as needed
    default:
        break;
    }
    
    return Multiplier;
}

float UYCRGameplayAbility::GetRarityMultiplier() const
{
    // Rarity multipliers based on your 10-tier system:
    // Common(1) to Godlike(10)
    const float RarityMultipliers[] = {
        1.0f,   // Common
        1.5f,   // Uncommon
        2.0f,   // Rare
        3.0f,   // Epic
        5.0f,   // Legendary
        8.0f,   // Mythic
        12.0f,  // Heroic
        18.0f,  // Glorious
        25.0f,  // Ultimate
        50.0f   // Godlike
    };
    
    int32 Index = FMath::Clamp(CurrentRarityLevel - 1, 0, 9);
    return RarityMultipliers[Index];
}

void UYCRGameplayAbility::ApplyYCRGameplayEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> EffectClass, float Level)
{
    if (!Target || !EffectClass)
    {
        return;
    }
    
    UAbilitySystemComponent* TargetASC = Target->FindComponentByClass<UAbilitySystemComponent>();
    if (!TargetASC)
    {
        return;
    }
    
    // Create effect context
    FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
    ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
    
    // Create and apply effect
    FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
        EffectClass, Level, ContextHandle);
        
    if (SpecHandle.IsValid())
    {
        // Apply rarity and element multipliers
        float FinalMultiplier = GetRarityMultiplier() * BaseDamageMultiplier;
        
        // Set damage value in the effect
        SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Damage"), FinalMultiplier);
        
        GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
            *SpecHandle.Data.Get(), TargetASC);
    }
}

bool UYCRGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    FGameplayTagContainer* OptionalRelevantTags) const
{
    // Add any YCR-specific cost checks here
    return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
}

void UYCRGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo) const
{
    // Apply any YCR-specific costs here
    Super::ApplyCost(Handle, ActorInfo, ActivationInfo);
}