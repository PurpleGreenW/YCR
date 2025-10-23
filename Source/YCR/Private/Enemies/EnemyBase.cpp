#include "Enemies/EnemyBase.h"
#include "Components/YCREnemyAIComponent.h"
#include "GAS/YCRAbilitySystemComponent.h"
#include "GAS/YCRAttributeSet.h"
#include "Character/CharacterPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayEffectExtension.h"

AEnemyBase::AEnemyBase()
{
    // Set default values
    PrimaryActorTick.bCanEverTick = true;

    // Create AI Component
    EnemyAIComponent = CreateDefaultSubobject<UYCREnemyAIComponent>(TEXT("EnemyAIComponent"));

    // Default tags
    MonsterTags.AddTag(FGameplayTag::RequestGameplayTag("Monster"));
}

void AEnemyBase::BeginPlay()
{
    Super::BeginPlay();

    // Initialize monster stats based on type and level
    InitializeMonsterStats();

    // Subscribe to health changes
    if (AttributeSet)
    {
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
            AttributeSet->GetHealthAttribute()).AddUObject(this, &AEnemyBase::OnHealthChanged);
    }
}

void AEnemyBase::InitializeAttributes()
{
    Super::InitializeAttributes();

    if (!AbilitySystemComponent || !AttributeSet)
        return;

    // Calculate stats based on level
    float HealthValue = CalculateStatForLevel(BaseStats.BaseHealth, 1.15f);
    float AttackValue = CalculateStatForLevel(BaseStats.BaseAttackPower, 1.08f);
    float DefenseValue = CalculateStatForLevel(BaseStats.BaseDefense, 1.05f);
    float SpeedValue = BaseStats.BaseMoveSpeed; // Speed doesn't scale with level

    // Apply base stats through GAS
    if (DefaultAttributes)
    {
        FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
        ContextHandle.AddSourceObject(this);

        FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
            DefaultAttributes, MonsterLevel, ContextHandle);

        if (SpecHandle.IsValid())
        {
            // Set base attribute values
            SpecHandle.Data.Get()->SetSetByCallerMagnitude(
                FGameplayTag::RequestGameplayTag("SetByCaller.MaxHealth"), HealthValue);
            SpecHandle.Data.Get()->SetSetByCallerMagnitude(
                FGameplayTag::RequestGameplayTag("SetByCaller.AttackPower"), AttackValue);
            SpecHandle.Data.Get()->SetSetByCallerMagnitude(
                FGameplayTag::RequestGameplayTag("SetByCaller.Armor"), DefenseValue);
            SpecHandle.Data.Get()->SetSetByCallerMagnitude(
                FGameplayTag::RequestGameplayTag("SetByCaller.MovementSpeed"), SpeedValue);
            SpecHandle.Data.Get()->SetSetByCallerMagnitude(
                FGameplayTag::RequestGameplayTag("SetByCaller.AttackSpeed"), BaseStats.BaseAttackSpeed);

            AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }
    }

    // Set current health to max health
    AttributeSet->SetHealth(AttributeSet->GetMaxHealth());

    // Update AI component stats
    if (EnemyAIComponent)
    {
        EnemyAIComponent->MoveSpeed = SpeedValue;
        EnemyAIComponent->AttackRange = 100.0f + (MonsterSize == EMonsterSize::Large ? 50.0f : 0.0f);
        EnemyAIComponent->ContactDamage = AttackValue;
    }
}

void AEnemyBase::GrantDefaultAbilities()
{
    Super::GrantDefaultAbilities();

    // Grant monster-specific abilities based on type
    // This would be expanded based on specific monster implementations
    if (AbilitySystemComponent && !DefaultAbilities.IsEmpty())
    {
        for (TSubclassOf<UYCRGameplayAbility>& Ability : DefaultAbilities)
        {
            if (Ability)
            {
                FGameplayAbilitySpec AbilitySpec(Ability, MonsterLevel, INDEX_NONE, this);
                AbilitySystemComponent->GiveAbility(AbilitySpec);
            }
        }
    }
}

void AEnemyBase::InitializeMonsterStats()
{
    // Set base stats based on monster type
    // This could be expanded with a data table lookup

    // Apply level scaling
    float HealthMultiplier = CalculateStatForLevel(1.0f, 1.15f);
    float AttackMultiplier = CalculateStatForLevel(1.0f, 1.08f);

    // Set movement component properties
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = BaseStats.BaseMoveSpeed;
    }
}

float AEnemyBase::CalculateStatForLevel(float BaseStat, float GrowthRate) const
{
    // Exponential growth formula similar to RO
    return BaseStat * FMath::Pow(GrowthRate, MonsterLevel - 1);
}

int32 AEnemyBase::CalculateExperienceReward() const
{
    // Base experience * level modifier
    float LevelModifier = 1.0f + (MonsterLevel * 0.1f);
    return FMath::RoundToInt(BaseStats.BaseExperience * LevelModifier);
}

int32 AEnemyBase::CalculateGoldReward() const
{
    // Base gold with some randomness
    float RandomModifier = FMath::RandRange(0.8f, 1.2f);
    float LevelModifier = 1.0f + (MonsterLevel * 0.15f);
    return FMath::RoundToInt(BaseStats.BaseGold * LevelModifier * RandomModifier);
}

void AEnemyBase::HandleDeath()
{
    Super::HandleDeath();

    // Drop loot
    if (bCanDropLoot)
    {
        OnDropLoot();
    }

    // Grant experience to player
    ACharacterPlayer* Player = Cast<ACharacterPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (Player)
    {
        // TODO: Grant experience through GAS
        int32 ExpReward = CalculateExperienceReward();
        int32 GoldReward = CalculateGoldReward();
        
        UE_LOG(LogTemp, Log, TEXT("Monster died: Granting %d EXP and %d Gold"), ExpReward, GoldReward);
    }

    // Destroy after delay
    SetLifeSpan(2.0f);
}

float AEnemyBase::GetElementalDamageModifier(EYCRElementType IncomingDamageElement) const
{
    // Use the attribute set's calculation
    if (AttributeSet)
    {
        return AttributeSet->CalculateElementalResistance(IncomingDamageElement, ElementType);
    }

    return 1.0f;
}

void AEnemyBase::OnHealthChanged(float OldValue, float NewValue)
{
    // React to damage
    if (NewValue < OldValue)
    {
        // Monster took damage - could trigger special behaviors
        float DamageTaken = OldValue - NewValue;
        
        // Check for assist behavior
        if (bIsAssistive)
        {
            // Alert nearby monsters of the same type
            TArray<AActor*> NearbyMonsters;
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyBase::StaticClass(), NearbyMonsters);
            
            for (AActor* Actor : NearbyMonsters)
            {
                AEnemyBase* OtherMonster = Cast<AEnemyBase>(Actor);
                if (OtherMonster && OtherMonster != this && 
                    OtherMonster->MonsterType == MonsterType &&
                    FVector::Dist(GetActorLocation(), OtherMonster->GetActorLocation()) <= 1000.0f)
                {
                    // Make them aggressive towards the attacker
                    OtherMonster->OnPlayerDetected(Cast<ACharacterPlayer>(GetInstigator()));
                }
            }
        }
    }
}

void AEnemyBase::OnPlayerDetected(ACharacterPlayer* Player)
{
    if (!bIsAggressive || !Player)
        return;

    // Set the AI component's target
    if (EnemyAIComponent)
    {
        // The AI component will handle the targeting internally
        UE_LOG(LogTemp, Log, TEXT("Monster detected player - becoming aggressive"));
    }
}

void AEnemyBase::OnPlayerLost()
{
    // Reset AI state
    UE_LOG(LogTemp, Log, TEXT("Monster lost player - returning to idle"));
}