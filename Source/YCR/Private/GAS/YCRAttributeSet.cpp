// Copyright YCR Project

#include "GAS/YCRAttributeSet.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffectExtension.h"

UYCRAttributeSet::UYCRAttributeSet()
{
    // Basis Attribute
    Health = 100.0f;
    MaxHealth = 100.0f;
    HealthRegeneration = 1.0f;
    
    Mana = 50.0f;
    MaxMana = 50.0f;
    ManaRegeneration = 2.0f;
    
    // Primär Attribute (Ragnarok Style)
    Strength = 1.0f;
    Agility = 1.0f;
    Vitality = 1.0f;
    Intelligence = 1.0f;
    Dexterity = 1.0f;
    Luck = 1.0f;
    
    // Kampf Attribute
    PhysicalDamage = 10.0f;
    MagicDamage = 10.0f;
    AttackSpeed = 1.0f;
    MovementSpeed = 400.0f;
    
    CriticalChance = 5.0f;
    CriticalMultiplier = 1.5f;
    
    DamageReduction = 0.0f;
    ElementalResistance = 0.0f;
    
    // Utility Attribute
    ExperienceMultiplier = 1.0f;
    GoldFindMultiplier = 1.0f;
    PickupRadius = 300.0f;
    
    // Projektil Attribute
    ProjectileCount = 1.0f;
    ProjectileSpeed = 1000.0f;
    ProjectilePierce = 0.0f;
    
    AreaOfEffect = 1.0f;
    Lifesteal = 0.0f;
    CooldownReduction = 0.0f;
}

void UYCRAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);
    
    // Clamp Health
    if (Attribute == GetHealthAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
    }
    // Clamp Mana
    else if (Attribute == GetManaAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
    }
    // Clamp Critical Chance
    else if (Attribute == GetCriticalChanceAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, 100.0f);
    }
    // Clamp Damage Reduction
    else if (Attribute == GetDamageReductionAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, 90.0f); // Max 90% damage reduction
    }
}

void UYCRAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);
    
    FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
    UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
    const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
    FGameplayTagContainer SpecAssetTags;
    Data.EffectSpec.GetAllAssetTags(SpecAssetTags);
    
    // Get the Target actor, which should be our owner
    AActor* TargetActor = nullptr;
    AController* TargetController = nullptr;
    if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
    {
        TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
        TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
    }
    
    // Handle attribute changes
    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        // Clamp health
        float NewHealth = FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth());
        SetHealth(NewHealth);
        
        // Check for death
        if (NewHealth <= 0.0f)
        {
            // Broadcast death event
            if (OnHealthZero.IsBound())
            {
                OnHealthZero.Broadcast();
            }
        }
    }
    else if (Data.EvaluatedData.Attribute == GetManaAttribute())
    {
        // Clamp mana
        float NewMana = FMath::Clamp(GetMana(), 0.0f, GetMaxMana());
        SetMana(NewMana);
    }
}

void UYCRAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
    Super::PostAttributeChange(Attribute, OldValue, NewValue);
    
    // Recalculate dependent attributes when primary stats change
    if (Attribute == GetStrengthAttribute())
    {
        // Update physical damage based on STR
        float NewPhysDmg = 10.0f + (GetStrength() * 2.0f);
        SetPhysicalDamage(NewPhysDmg);
    }
    else if (Attribute == GetAgilityAttribute())
    {
        // Update attack speed and movement speed based on AGI
        float NewAtkSpeed = 1.0f + (GetAgility() * 0.02f);
        SetAttackSpeed(NewAtkSpeed);
        
        float NewMoveSpeed = 400.0f + (GetAgility() * 5.0f);
        SetMovementSpeed(NewMoveSpeed);
    }
    else if (Attribute == GetVitalityAttribute())
    {
        // Update max health and health regen based on VIT
        float NewMaxHealth = 100.0f + (GetVitality() * 20.0f);
        SetMaxHealth(NewMaxHealth);
        
        float NewHealthRegen = 1.0f + (GetVitality() * 0.5f);
        SetHealthRegeneration(NewHealthRegen);
    }
    else if (Attribute == GetIntelligenceAttribute())
    {
        // Update magic damage and max mana based on INT
        float NewMagicDmg = 10.0f + (GetIntelligence() * 3.0f);
        SetMagicDamage(NewMagicDmg);
        
        float NewMaxMana = 50.0f + (GetIntelligence() * 10.0f);
        SetMaxMana(NewMaxMana);
    }
    else if (Attribute == GetDexterityAttribute())
    {
        // Update critical chance and projectile speed based on DEX
        float NewCritChance = 5.0f + (GetDexterity() * 0.5f);
        SetCriticalChance(NewCritChance);
        
        float NewProjSpeed = 1000.0f + (GetDexterity() * 20.0f);
        SetProjectileSpeed(NewProjSpeed);
    }
    else if (Attribute == GetLuckAttribute())
    {
        // Update critical multiplier and gold find based on LUK
        float NewCritMult = 1.5f + (GetLuck() * 0.01f);
        SetCriticalMultiplier(NewCritMult);
        
        float NewGoldFind = 1.0f + (GetLuck() * 0.02f);
        SetGoldFindMultiplier(NewGoldFind);
    }
}

// Helper Funktionen für berechnete Werte
float UYCRAttributeSet::GetEffectivePhysicalDamage() const
{
    // Basis-Schaden + STR-Bonus + Modifikatoren
    return GetPhysicalDamage();
}

float UYCRAttributeSet::GetEffectiveMagicDamage() const
{
    // Basis-Schaden + INT-Bonus + Modifikatoren
    return GetMagicDamage();
}

float UYCRAttributeSet::GetEffectiveAttackSpeed() const
{
    // Basis-Speed * AGI-Bonus * Modifikatoren
    return GetAttackSpeed();
}

float UYCRAttributeSet::GetEffectiveCriticalChance() const
{
    // Basis-Chance + DEX-Bonus + LUK-Bonus
    return FMath::Clamp(GetCriticalChance(), 0.0f, 100.0f);
}