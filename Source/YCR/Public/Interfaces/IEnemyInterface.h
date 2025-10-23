#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "Enums/EYCRMonsterTypes.h"
#include "Enums/EYCRElements.h"
#include "Enums/EYCRSize.h"
#include "Enums/EYCRRace.h"
#include "IEnemyInterface.generated.h"

// Forward declarations
class ACharacterPlayer;
class UGameplayEffect;
struct FGameplayEffectSpec;

UINTERFACE(MinimalAPI, Blueprintable)
class UEnemyInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * Interface for all enemy characters in YCR
 * Provides common enemy functionality for AI behavior, aggro system, and combat
 */
class YCR_API IEnemyInterface
{
    GENERATED_BODY()

public:
    // ============= Core Enemy Functions =============
    
    // Get the enemy's monster type
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy")
    EYCRMonsterType GetMonsterType() const;
    
    // Get the enemy's element type and level
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy")
    EYCRElementType GetElementType() const;
    
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy")
    int32 GetElementLevel() const;
    
    // Get enemy level
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy")
    int32 GetEnemyLevel() const;

    // ============= Aggro System =============
    
    // Called when a player enters aggro range
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|Aggro")
    void OnPlayerEnterAggroRange(ACharacterPlayer* Player);
    
    // Called when a player leaves aggro range
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|Aggro")
    void OnPlayerExitAggroRange(ACharacterPlayer* Player);
    
    // Check if enemy is currently aggressive
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|Aggro")
    bool IsAggressive() const;
    
    // Check if enemy assists other enemies
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|Aggro")
    bool IsAssistive() const;
    
    // Get current aggro target
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|Aggro")
    ACharacterPlayer* GetCurrentTarget() const;
    
    // Set aggro target
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|Aggro")
    void SetAggroTarget(ACharacterPlayer* NewTarget);

    // ============= Combat Functions =============
    
    // Called when enemy takes damage
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|Combat")
    void OnTakeDamage(float DamageAmount, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec);
    
    // Get enemy's threat level (for player prioritization)
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|Combat")
    float GetThreatLevel() const;
    
    // Check if enemy can attack
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|Combat")
    bool CanAttack() const;
    
    // Perform attack
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|Combat")
    void PerformAttack();

    // ============= Reward System =============
    
    // Get experience reward for killing this enemy
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|Rewards")
    int32 GetExperienceReward() const;
    
    // Get gold reward for killing this enemy
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|Rewards")
    int32 GetGoldReward() const;
    
    // Can this enemy drop loot?
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|Rewards")
    bool CanDropLoot() const;
    
    // Drop loot on death
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|Rewards")
    void DropLoot();

    // ============= AI Behavior =============
    
    // Get enemy behavior tags (for AI decision making)
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|AI")
    FGameplayTagContainer GetBehaviorTags() const;
    
    // Check if enemy has specific behavior tag
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|AI")
    bool HasBehaviorTag(FGameplayTag Tag) const;
    
    // Get enemy's detection range
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|AI")
    float GetDetectionRange() const;
    
    // Get enemy's attack range
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|AI")
    float GetAttackRange() const;

    // ============= State Management =============
    
    // Check if enemy is in combat
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|State")
    bool IsInCombat() const;
    
    // Enter combat state
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|State")
    void EnterCombat();
    
    // Exit combat state
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|State")
    void ExitCombat();
    
    // Check if enemy is stunned
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|State")
    bool IsStunned() const;

    // ============= Special Properties =============
    
    // Get enemy size (affects some mechanics)
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|Properties")
    EYCRSize GetMonsterSize() const;
    
    // Get enemy race (affects damage calculations)
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|Properties")
    EYCRRace GetMonsterRace() const;

    // Check if enemy is a boss
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|Properties")
    bool IsMiniBoss() const;
    
    // Check if enemy is a boss
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|Properties")
    bool IsBoss() const;
    
    // Check if enemy is elite/champion
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|Properties")
    bool IsElite() const;

    // ============= Events =============
    
    // Called when enemy spawns
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|Events")
    void OnEnemySpawned();
    
    // Called when enemy dies
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|Events")
    void OnEnemyDeath(AActor* Killer);
    
    // Called when enemy is alerted
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Enemy|Events")
    void OnAlerted(AActor* AlertCauser);
};