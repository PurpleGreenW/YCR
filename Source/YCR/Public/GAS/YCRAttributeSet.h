#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "YCR/Public/Enums/EYCRElements.h"
#include "YCRAttributeSet.generated.h"

// Helper macros for attribute accessors
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class YCR_API UYCRAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UYCRAttributeSet();

    // Attribute modification callbacks
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
    virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

    // Primary Attributes (Raw Stats)
    UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
    FGameplayAttributeData Strength;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, Strength)

    UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
    FGameplayAttributeData Agility;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, Agility)

    UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
    FGameplayAttributeData Vitality;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, Vitality)

    UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
    FGameplayAttributeData Intelligence;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, Intelligence)

    UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
    FGameplayAttributeData Dexterity;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, Dexterity)

    UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
    FGameplayAttributeData Luck;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, Luck)

    // Vital Attributes
    UPROPERTY(BlueprintReadOnly, Category = "Vital Attributes")
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, Health)

    UPROPERTY(BlueprintReadOnly, Category = "Vital Attributes")
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, MaxHealth)

    // Combat Attributes
    UPROPERTY(BlueprintReadOnly, Category = "Combat Attributes")
    FGameplayAttributeData AttackPower;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, AttackPower)

    UPROPERTY(BlueprintReadOnly, Category = "Combat Attributes")
    FGameplayAttributeData AttackSpeed;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, AttackSpeed)

    UPROPERTY(BlueprintReadOnly, Category = "Combat Attributes")
    FGameplayAttributeData CriticalChance;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, CriticalChance)

    UPROPERTY(BlueprintReadOnly, Category = "Combat Attributes")
    FGameplayAttributeData CriticalDamage;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, CriticalDamage)

    UPROPERTY(BlueprintReadOnly, Category = "Combat Attributes")
    FGameplayAttributeData Armor;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, Armor)

    // Projectile Attributes
    UPROPERTY(BlueprintReadOnly, Category = "Projectile Attributes")
    FGameplayAttributeData ProjectileSpeed;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, ProjectileSpeed)

    UPROPERTY(BlueprintReadOnly, Category = "Projectile Attributes")
    FGameplayAttributeData ProjectilePierce;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, ProjectilePierce)

    UPROPERTY(BlueprintReadOnly, Category = "Projectile Attributes")
    FGameplayAttributeData ProjectileBounce;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, ProjectileBounce)

    // Survivor Attributes
    UPROPERTY(BlueprintReadOnly, Category = "Survivor Attributes")
    FGameplayAttributeData Lifesteal;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, Lifesteal)

    UPROPERTY(BlueprintReadOnly, Category = "Survivor Attributes")
    FGameplayAttributeData MovementSpeed;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, MovementSpeed)

    UPROPERTY(BlueprintReadOnly, Category = "Survivor Attributes")
    FGameplayAttributeData PickupRange;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, PickupRange)

    // Utility Attributes
    UPROPERTY(BlueprintReadOnly, Category = "Utility Attributes")
    FGameplayAttributeData ExperienceMultiplier;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, ExperienceMultiplier)

    UPROPERTY(BlueprintReadOnly, Category = "Utility Attributes")
    FGameplayAttributeData GoldFindMultiplier;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, GoldFindMultiplier)

protected:
    // Helper functions for damage calculation
    float CalculateDamageReduction() const;
    float CalculateElementalResistance(EYCRElementType DamageElement, EYCRElementType DefenderElement) const;
};