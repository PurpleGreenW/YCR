// Copyright YCR Project

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterAttributeData.h"
#include "YCR/Public/Enums/EYCRJobClasses.h"
#include "YCR/Public/Enums/EYCRElementTypes.h"
#include "JobClassData.generated.h"

/**
 * DataTable Struktur für Job-Klassen Definitionen
 */
USTRUCT(BlueprintType)
struct FJobClassData : public FTableRowBase
{
    GENERATED_BODY()

    /** Display Name der Klasse */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
    FText ClassName;

    /** Beschreibung der Klasse */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General", meta = (MultiLine = true))
    FText ClassDescription;

    /** Job Class Enum Type */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
    EJobClass JobClassType = EJobClass::Swordsman;

    /** Element-Affinität der Klasse */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
    EElementType ClassElement = EElementType::Neutral;

    /** Icon für UI */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSoftObjectPtr<UTexture2D> ClassIcon;

    /** Basis-Attribute dieser Klasse */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FCharacterAttributeData BaseAttributes;

    /** Start-Ability dieser Klasse */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
    TSubclassOf<class UYCRGameplayAbility> StartingOffensiveAbility;

    /** Passive Ability dieser Klasse */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
    TSubclassOf<class UYCRGameplayAbility> StartingPassiveAbility;

    /** Fortgeschrittene Klassen (Knight, Crusader etc.) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advancement")
    TArray<FName> AdvancedClassOptions;

    /** Level für Class Advancement */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advancement")
    int32 AdvancementLevel = 40;

    /** Freischaltungs-Voraussetzungen */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unlock")
    int32 RequiredAccountLevel = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unlock")
    int32 RequiredEssenceCost = 0;

    /** Ist diese Klasse von Anfang an verfügbar? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unlock")
    bool bStarterClass = false;
};