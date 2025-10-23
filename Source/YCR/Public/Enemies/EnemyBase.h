#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interfaces/IEnemyInterface.h"
#include "Enums/EYCRMonsterTypes.h"
#include "Enums/EYCRSize.h"
#include "Enums/EYCRElements.h"
#include "EnemyBase.generated.h"

UCLASS()
class YCR_API AEnemyBase : public ACharacterBase, public IEnemyInterface
{
    GENERATED_BODY()

public:
    AEnemyBase();

protected:
    virtual void BeginPlay() override;
    virtual void PossessedBy(AController* NewController) override;

    // Enemy-specific properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
    EYCRMonsterType MonsterType;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
    FName MonsterID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Stats")
    float BaseExperienceReward;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Stats")
    float BaseGoldDrop;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Stats")
    float AggroRange;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Stats")
    float AttackRange;

    // Monster Size (affects stats and knockback)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Stats")
    EYCRSize MonsterSize;

    // AI Component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|AI")
    class UYCREnemyAIComponent* EnemyAIComponent;

public:
    // IEnemyInterface implementation
    virtual float GetExperienceReward() const override { return BaseExperienceReward; }
    virtual float GetGoldDrop() const override { return BaseGoldDrop; }
    virtual EYCRMonsterType GetMonsterType() const override { return MonsterType; }

    // Enemy-specific functions
    UFUNCTION(BlueprintCallable, Category = "Enemy")
    void InitializeMonsterStats(const struct FYCRMonsterData& MonsterData);

protected:
    // Override CharacterBase functions
    virtual void InitializeAttributes() override;
    virtual void OnDeath() override;

    // Apply monster-specific stat modifiers
    void ApplyMonsterTypeModifiers();
    void ApplySizeModifiers();
};