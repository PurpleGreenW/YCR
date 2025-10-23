// D:\Unreal Projects\YCR\Source\YCR\Public\Character\CharacterBase.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CharacterBase.generated.h"

// Forward Declarations
class UYCRAbilitySystemComponent;
class UYCRAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
enum class EElementTypes : uint8;

/**
 * Base character class for all characters in YCR
 * Handles GAS integration, basic stats, and element system
 */
UCLASS(Abstract)
class YCR_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    ACharacterBase();

    // =====================================================
    // Ability System Interface
    // =====================================================
    virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
    virtual void BeginPlay() override;

    // =====================================================
    // Components
    // =====================================================
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "YCR|GAS", meta = (AllowPrivateAccess = "true"))
    class UYCRAbilitySystemComponent* AbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "YCR|GAS", meta = (AllowPrivateAccess = "true"))
    class UYCRAttributeSet* AttributeSet;

    // =====================================================
    // Abilities and Effects
    // =====================================================
    
    /** Abilities to grant when this character spawns */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YCR|Abilities")
    TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

    /** Passive effects to apply on spawn */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YCR|Abilities")
    TArray<TSubclassOf<UGameplayEffect>> DefaultPassiveEffects;

    /** Effects to apply based on character level */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YCR|Abilities")
    TMap<int32, TSubclassOf<UGameplayEffect>> DefaultLevelEffects;

    // =====================================================
    // Character Stats
    // =====================================================
    
    /** Current character level */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "YCR|Stats", meta = (ClampMin = "1", ClampMax = "100"))
    int32 CharacterLevel;

    /** Character's element type for damage calculations */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YCR|Stats")
    EElementTypes CharacterElement;

    /** Is this character dead? */
    UPROPERTY(BlueprintReadOnly, Category = "YCR|Stats")
    bool bIsDead;

public:
    // =====================================================
    // Public Functions
    // =====================================================
    
    /** Set the character level and apply appropriate effects */
    UFUNCTION(BlueprintCallable, Category = "YCR|Stats")
    virtual void SetCharacterLevel(int32 NewLevel);

    /** Get health as percentage (0-1) */
    UFUNCTION(BlueprintCallable, Category = "YCR|Stats")
    float GetHealthPercent() const;

    /** Apply damage to this character */
    UFUNCTION(BlueprintCallable, Category = "YCR|Combat")
    virtual void ReceiveDamage(float DamageAmount, FDamageEvent const& DamageEvent, 
        AController* EventInstigator, AActor* DamageCauser);

    /** Check if character is alive */
    UFUNCTION(BlueprintCallable, Category = "YCR|Stats")
    bool IsDead() const;

protected:
    // =====================================================
    // Protected Functions
    // =====================================================
    
    /** Initialize the ability system component */
    virtual void InitializeAbilitySystem();

    /** Grant initial abilities */
    virtual void GrantDefaultAbilities();

    /** Apply initial effects */
    virtual void ApplyDefaultEffects();

    /** Called when health changes */
    virtual void HandleHealthChanged(const struct FOnAttributeChangeData& Data);

    /** Called when max health changes */
    virtual void HandleMaxHealthChanged(const struct FOnAttributeChangeData& Data);

    /** Handle death */
    virtual void Die();
};