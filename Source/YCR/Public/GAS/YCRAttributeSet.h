// Copyright YCR Project

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "YCRAttributeSet.generated.h"

// Makros für Attribute Accessors
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Basis AttributeSet für alle YCR Charaktere
 * Enthält alle grundlegenden Stats inspiriert von Ragnarok Online
 */
UCLASS()
class YCR_API UYCRAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UYCRAttributeSet();

    // =====================================================
    // Primäre Attribute (Ragnarok Style)
    // =====================================================
    
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

    // =====================================================
    // Lebenspunkte
    // =====================================================
    
    UPROPERTY(BlueprintReadOnly, Category = "Health")
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, Health)

    UPROPERTY(BlueprintReadOnly, Category = "Health")
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, MaxHealth)

    UPROPERTY(BlueprintReadOnly, Category = "Health")
    FGameplayAttributeData HealthRegeneration;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, HealthRegeneration)

    // =====================================================
    // Kampfwerte
    // =====================================================
    
    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    FGameplayAttributeData PhysicalDamage;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, PhysicalDamage)

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    FGameplayAttributeData MagicalDamage;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, MagicalDamage)

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    FGameplayAttributeData AttackSpeed;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, AttackSpeed)

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    FGameplayAttributeData CriticalChance;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, CriticalChance)

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    FGameplayAttributeData CriticalDamageMultiplier;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, CriticalDamageMultiplier)

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    FGameplayAttributeData Defense;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, Defense)

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    FGameplayAttributeData MagicDefense;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, MagicDefense)

    // =====================================================
    // Bewegung
    // =====================================================
    
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    FGameplayAttributeData MovementSpeed;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, MovementSpeed)

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    FGameplayAttributeData DodgeChance;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, DodgeChance)

    // =====================================================
    // Vampire Survivors Style Attribute
    // =====================================================
    
    UPROPERTY(BlueprintReadOnly, Category = "Survivor")
    FGameplayAttributeData PickupRadius;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, PickupRadius)

    UPROPERTY(BlueprintReadOnly, Category = "Survivor")
    FGameplayAttributeData ExperienceMultiplier;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, ExperienceMultiplier)

    UPROPERTY(BlueprintReadOnly, Category = "Survivor")
    FGameplayAttributeData GoldFindMultiplier;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, GoldFindMultiplier)

    UPROPERTY(BlueprintReadOnly, Category = "Survivor")
    FGameplayAttributeData ProjectileCount;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, ProjectileCount)

    UPROPERTY(BlueprintReadOnly, Category = "Survivor")
    FGameplayAttributeData AreaOfEffect;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, AreaOfEffect)

    UPROPERTY(BlueprintReadOnly, Category = "Survivor")
    FGameplayAttributeData Cooldown;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, Cooldown)

    UPROPERTY(BlueprintReadOnly, Category = "Survivor")
    FGameplayAttributeData Duration;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, Duration)

    UPROPERTY(BlueprintReadOnly, Category = "Survivor")
    FGameplayAttributeData Pierce;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, Pierce)

    UPROPERTY(BlueprintReadOnly, Category = "Survivor")
    FGameplayAttributeData Lifesteal;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, Lifesteal)

    // =====================================================
    // Overrides
    // =====================================================
    
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

protected:
    // Helper Funktionen
    void UpdateDerivedAttributes();
    
    // Helper für berechnete Werte
    float GetEffectivePhysicalDamage() const;
    float GetEffectiveMagicalDamage() const;
    float GetEffectiveAttackSpeed() const;
};