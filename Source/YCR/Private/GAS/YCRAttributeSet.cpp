#include "GAS/YCRAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Character/CharacterBase.h"
#include "YCR/Public/Enums/EYCRElements.h"

UYCRAttributeSet::UYCRAttributeSet()
{
    // Initialize Primary Attributes
    Strength.SetBaseValue(10.0f);
    Agility.SetBaseValue(10.0f);
    Vitality.SetBaseValue(10.0f);
    Intelligence.SetBaseValue(10.0f);
    Dexterity.SetBaseValue(10.0f);
    Luck.SetBaseValue(10.0f);

    // Initialize Vital Attributes
    MaxHealth.SetBaseValue(100.0f);
    Health.SetBaseValue(MaxHealth.GetBaseValue());

    // Initialize Combat Attributes
    AttackPower.SetBaseValue(10.0f);
    AttackSpeed.SetBaseValue(1.0f);
    CriticalChance.SetBaseValue(5.0f);
    CriticalDamage.SetBaseValue(150.0f);
    Armor.SetBaseValue(0.0f);

    // Initialize Projectile Attributes
    ProjectileSpeed.SetBaseValue(1000.0f);
    ProjectilePierce.SetBaseValue(0.0f);
    ProjectileBounce.SetBaseValue(0.0f);

    // Initialize Survivor Attributes
    Lifesteal.SetBaseValue(0.0f);
    MovementSpeed.SetBaseValue(300.0f);
    PickupRange.SetBaseValue(100.0f);

    // Initialize Utility Attributes
    ExperienceMultiplier.SetBaseValue(1.0f);
    GoldFindMultiplier.SetBaseValue(1.0f);
}

void UYCRAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);

    // Clamp attribute values
    if (Attribute == GetHealthAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
    }
    else if (Attribute == GetAttackSpeedAttribute())
    {
        NewValue = FMath::Max(NewValue, 0.1f); // Minimum attack speed
    }
    else if (Attribute == GetCriticalChanceAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, 100.0f);
    }
    else if (Attribute == GetProjectilePierceAttribute())
    {
        NewValue = FMath::Max(NewValue, 0.0f);
    }
    else if (Attribute == GetProjectileBounceAttribute())
    {
        NewValue = FMath::Max(NewValue, 0.0f);
    }
}

void UYCRAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    // Handle health changes
    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
    }
}

void UYCRAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
    Super::PostAttributeChange(Attribute, OldValue, NewValue);

    // Update dependent attributes when primary stats change
    if (Attribute == GetVitalityAttribute())
    {
        // Vitality affects MaxHealth (10 HP per Vitality point)
        float NewMaxHealth = 100.0f + (GetVitality() * 10.0f);
        SetMaxHealth(NewMaxHealth);
        
        // Scale current health proportionally
        if (OldValue > 0)
        {
            float HealthPercentage = GetHealth() / (100.0f + (OldValue * 10.0f));
            SetHealth(HealthPercentage * NewMaxHealth);
        }
    }
    else if (Attribute == GetStrengthAttribute())
    {
        // Strength affects AttackPower
        SetAttackPower(10.0f + (GetStrength() * 2.0f));
    }
    else if (Attribute == GetAgilityAttribute())
    {
        // Agility affects AttackSpeed and MovementSpeed
        SetAttackSpeed(1.0f + (GetAgility() * 0.02f)); // 2% attack speed per AGI
        SetMovementSpeed(300.0f + (GetAgility() * 2.0f)); // +2 movement speed per AGI
    }
    else if (Attribute == GetDexterityAttribute())
    {
        // Dexterity affects CriticalChance
        SetCriticalChance(5.0f + (GetDexterity() * 0.2f)); // 0.2% crit per DEX
    }
    else if (Attribute == GetLuckAttribute())
    {
        // Luck affects CriticalDamage and drop rates
        SetCriticalDamage(150.0f + (GetLuck() * 1.0f)); // 1% crit damage per LUK
        SetGoldFindMultiplier(1.0f + (GetLuck() * 0.01f)); // 1% gold find per LUK
    }
}

float UYCRAttributeSet::CalculateDamageReduction() const
{
    // Damage reduction formula based on Armor and Vitality
    // Similar to many survivor games: DR = Armor / (Armor + 100)
    float TotalArmor = GetArmor() + (GetVitality() * 0.5f); // Vitality adds to armor
    return TotalArmor / (TotalArmor + 100.0f);
}

float UYCRAttributeSet::CalculateElementalResistance(EYCRElementType DamageElement, EYCRElementType DefenderElement) const
{
    // Based on Ragnarok Online element table
    // Returns damage multiplier (1.0 = normal, 0.5 = resistant, 2.0 = weak)
    
    // Same element = 25% reduction
    if (DamageElement == DefenderElement)
    {
        return 0.75f;
    }

    // Element advantage/disadvantage system
    switch (DefenderElement)
    {
        case EYCRElementType::Fire:
            if (DamageElement == EYCRElementType::Water) return 2.0f; // Fire weak to Water
            if (DamageElement == EYCRElementType::Earth) return 0.5f; // Fire strong vs Earth
            if (DamageElement == EYCRElementType::Wind) return 0.5f; // Fire strong vs Wind
            break;
            
        case EYCRElementType::Water:
            if (DamageElement == EYCRElementType::Wind) return 2.0f; // Water weak to Wind
            if (DamageElement == EYCRElementType::Fire) return 0.5f; // Water strong vs Fire
            if (DamageElement == EYCRElementType::Earth) return 1.0f; // Neutral
            break;
            
        case EYCRElementType::Wind:
            if (DamageElement == EYCRElementType::Earth) return 2.0f; // Wind weak to Earth
            if (DamageElement == EYCRElementType::Water) return 0.5f; // Wind strong vs Water
            if (DamageElement == EYCRElementType::Fire) return 1.0f; // Neutral
            break;
            
        case EYCRElementType::Earth:
            if (DamageElement == EYCRElementType::Fire) return 2.0f; // Earth weak to Fire
            if (DamageElement == EYCRElementType::Wind) return 0.5f; // Earth strong vs Wind
            if (DamageElement == EYCRElementType::Water) return 1.0f; // Neutral
            break;
            
        case EYCRElementType::Holy:
            if (DamageElement == EYCRElementType::Shadow) return 2.0f; // Holy weak to Shadow
            if (DamageElement == EYCRElementType::Undead) return 0.25f; // Holy very strong vs Undead
            break;
            
        case EYCRElementType::Shadow:
            if (DamageElement == EYCRElementType::Holy) return 2.0f; // Shadow weak to Holy
            break;
            
        case EYCRElementType::Undead:
            if (DamageElement == EYCRElementType::Holy) return 4.0f; // Undead very weak to Holy
            if (DamageElement == EYCRElementType::Fire) return 1.5f; // Undead weak to Fire
            if (DamageElement == EYCRElementType::Shadow) return 0.5f; // Undead resistant to Shadow
            break;
    }
    
    // Default neutral damage
    return 1.0f;
}