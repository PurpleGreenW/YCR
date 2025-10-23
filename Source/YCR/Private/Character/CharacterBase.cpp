// Copyright YCR Project

#include "Character/CharacterBase.h"
#include "YCR/Public/GAS/YCRAbilitySystemComponent.h"
#include "YCR/Public/GAS/YCRAttributeSet.h"
#include "YCR/Public/GAS/YCRGameplayAbility.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "YCR/Public/Enums/EYCRElements.h"
#include "AbilitySystemBlueprintLibrary.h"

ACharacterBase::ACharacterBase()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create Ability System Component
    AbilitySystemComponent = CreateDefaultSubobject<UYCRAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    // Create Attribute Set
    AttributeSet = CreateDefaultSubobject<UYCRAttributeSet>(TEXT("AttributeSet"));

    // Default values
    CharacterLevel = 1;
    CharacterElement = EYCRElementType::Neutral;
    bIsDead = false;

    // Configure character movement (Survivor-style)
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}

void ACharacterBase::BeginPlay()
{
    Super::BeginPlay();
    
    InitializeAbilitySystem();
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void ACharacterBase::InitializeAbilitySystem()
{
    if (!AbilitySystemComponent)
    {
        return;
    }

    // Initialize ASC with this actor as owner and avatar
    AbilitySystemComponent->InitAbilityActorInfo(this, this);

    // Grant abilities
    GrantDefaultAbilities();

    // Apply default effects
    ApplyDefaultEffects();

    // Bind to attribute changes
    if (AttributeSet)
    {
        // Health changes
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
            AttributeSet->GetHealthAttribute()
        ).AddUObject(this, &ACharacterBase::HandleHealthChanged);

        // Max Health changes
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
            AttributeSet->GetMaxHealthAttribute()
        ).AddUObject(this, &ACharacterBase::HandleMaxHealthChanged);
    }
}

void ACharacterBase::GrantDefaultAbilities()
{
    if (!AbilitySystemComponent)
    {
        return;
    }

    // Grant each default ability
    for (const TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
    {
        if (AbilityClass)
        {
            FGameplayAbilitySpec AbilitySpec(AbilityClass, CharacterLevel, INDEX_NONE, this);
            AbilitySystemComponent->GiveAbility(AbilitySpec);
        }
    }
}

void ACharacterBase::ApplyDefaultEffects()
{
    if (!AbilitySystemComponent)
    {
        return;
    }

    // Apply passive effects
    for (const TSubclassOf<UGameplayEffect>& EffectClass : DefaultPassiveEffects)
    {
        if (EffectClass)
        {
            FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
            ContextHandle.AddSourceObject(this);
            
            FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
                EffectClass, CharacterLevel, ContextHandle
            );
            
            if (SpecHandle.IsValid())
            {
                AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
            }
        }
    }

    // Apply level-based effects
    if (DefaultLevelEffects.Contains(CharacterLevel))
    {
        TSubclassOf<UGameplayEffect> LevelEffect = DefaultLevelEffects[CharacterLevel];
        if (LevelEffect)
        {
            FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
            ContextHandle.AddSourceObject(this);
            
            FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
                LevelEffect, CharacterLevel, ContextHandle
            );
            
            if (SpecHandle.IsValid())
            {
                AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
            }
        }
    }
}

void ACharacterBase::SetCharacterLevel(int32 NewLevel)
{
    if (CharacterLevel != NewLevel && NewLevel > 0)
    {
        CharacterLevel = NewLevel;
        
        // Reapply level-based effects
        ApplyDefaultEffects();
    }
}

float ACharacterBase::GetHealthPercent() const
{
    if (!AttributeSet)
    {
        return 0.0f;
    }

    const float Health = AttributeSet->GetHealth();
    const float MaxHealth = AttributeSet->GetMaxHealth();

    return MaxHealth > 0.0f ? Health / MaxHealth : 0.0f;
}

void ACharacterBase::ReceiveDamage(float DamageAmount, FDamageEvent const& DamageEvent, 
    AController* EventInstigator, AActor* DamageCauser)
{
    // TODO: Implement damage handling through GAS
    // This will create a GameplayEffect for damage and apply it
}

bool ACharacterBase::IsDead() const
{
    return bIsDead;
}

void ACharacterBase::HandleHealthChanged(const FOnAttributeChangeData& Data)
{
    if (Data.NewValue <= 0.0f && !bIsDead)
    {
        Die();
    }
}

void ACharacterBase::HandleMaxHealthChanged(const FOnAttributeChangeData& Data)
{
    // Update UI or other systems when max health changes
}

void ACharacterBase::Die()
{
    if (bIsDead)
    {
        return;
    }

    bIsDead = true;

    // Disable collision
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    // Disable movement
    GetCharacterMovement()->DisableMovement();

    // TODO: Play death animation, spawn loot, etc.
}