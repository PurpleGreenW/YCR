// Copyright YCR Project

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterAttributeData.generated.h"

/**
 * Basis-Struktur für Character Attribute im DataTable
 * Kann im Editor für verschiedene Character-Klassen angepasst werden
 */
USTRUCT(BlueprintType)
struct FCharacterAttributeData : public FTableRowBase
{
    GENERATED_BODY()

    // ========== Basis Stats ==========
    
    /** Basis Lebenspunkte */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
    float BaseHealth = 100.0f;

    /** Basis Mana/Energie für Skills */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
    float BaseMana = 50.0f;

    /** Basis Bewegungsgeschwindigkeit */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
    float BaseMoveSpeed = 400.0f;

    // ========== Offensive Stats ==========
    
    /** Basis Angriffskraft */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offensive")
    float BaseAttackPower = 10.0f;

    /** Angriffe pro Sekunde */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offensive")
    float BaseAttackSpeed = 1.0f;

    /** Kritische Trefferchance in Prozent */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offensive", meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float BaseCritChance = 5.0f;

    /** Kritischer Schadens-Multiplikator */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offensive")
    float BaseCritDamage = 1.5f;

    /** Projektilgeschwindigkeit Multiplikator */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offensive")
    float BaseProjectileSpeed = 1.0f;

    // ========== Defensive Stats ==========
    
    /** Physische Verteidigung */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defensive")
    float BaseDefense = 5.0f;

    /** Magische Resistenz */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defensive")
    float BaseMagicResist = 5.0f;

    /** Ausweich-Chance in Prozent */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defensive", meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float BaseEvasion = 0.0f;

    // ========== Utility Stats ==========
    
    /** Pickup-Radius für Items/Gold/Exp */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility")
    float BasePickupRadius = 100.0f;

    /** Erfahrungs-Multiplikator */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility")
    float BaseExpGain = 1.0f;

    /** Gold-Find Multiplikator */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility")
    float BaseGoldFind = 1.0f;

    /** Glück für bessere Item-Drops */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility")
    float BaseLuck = 0.0f;

    // ========== Special Stats ==========
    
    /** Area of Effect Größen-Multiplikator */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special")
    float BaseAreaSize = 1.0f;

    /** Durchschlag - Anzahl zusätzlicher Gegner die getroffen werden */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special")
    int32 BasePiercing = 0;

    /** Anzahl zusätzlicher Projektile */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special")
    int32 BaseProjectileCount = 0;

    /** Skill Cooldown Reduktion in Prozent */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special", meta = (ClampMin = "0.0", ClampMax = "80.0"))
    float BaseCooldownReduction = 0.0f;

    /** Lebensregeneration pro Sekunde */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special")
    float BaseHealthRegen = 0.0f;

    /** Mana-Regeneration pro Sekunde */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special")
    float BaseManaRegen = 1.0f;

    // ========== Constructor ==========
    
    FCharacterAttributeData()
    {
        // Default Werte sind bereits oben definiert
    }
};