// D:\Unreal Projects\YCR\Source\YCR\Public\GAS\YCRAttributeSet.h
#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "YCRAttributeSet.generated.h"

// Macro helper for attribute accessors
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Attribute set for YCR characters
 * Contains all gameplay attributes like health, damage, speed etc.
 */
UCLASS()
class YCR_API UYCRAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UYCRAttributeSet();

    // Replication
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // =====================================================
    // Vital Attributes
    // =====================================================
    
    UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, Health)

    UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UYCRAttributeSet, MaxHealth)

    // =====================================================
    // Primary Attributes (Ragnarok Style)
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

protected:
    // Replication functions
    UFUNCTION()
    virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

    UFUNCTION()
    virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
};