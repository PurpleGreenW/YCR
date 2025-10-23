#include "YCR/Public/Components/StatusEffectComponent.h"
#include "YCR/Public/Interfaces/IDamageableInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DamageEvents.h"

UStatusEffectComponent::UStatusEffectComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UStatusEffectComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UStatusEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    ProcessStatusEffects(DeltaTime);
}

void UStatusEffectComponent::ProcessStatusEffects(float DeltaTime)
{
    // Process active effects in reverse order for safe removal
    for (int32 i = ActiveEffects.Num() - 1; i >= 0; i--)
    {
        FStatusEffect& Effect = ActiveEffects[i];
        
        // Apply tick damage
        if (Effect.TickDamage > 0.0f)
        {
            if (IDamageableInterface* Damageable = Cast<IDamageableInterface>(GetOwner()))
            {
                FDamageEvent DamageEvent;
                Damageable->Execute_ReceiveDamage(GetOwner(), Effect.TickDamage * DeltaTime, DamageEvent, nullptr, nullptr);
            }
        }
        
        // Apply slow
        if (Effect.SlowPercent > 0.0f)
        {
            if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
            {
                Character->GetCharacterMovement()->MaxWalkSpeed *= (1.0f - Effect.SlowPercent / 100.0f);
            }
        }
        
        // Update duration
        Effect.Duration -= DeltaTime;
        if (Effect.Duration <= 0.0f)
        {
            ActiveEffects.RemoveAt(i);
        }
    }
}

void UStatusEffectComponent::ApplyStatusEffect(const FStatusEffect& NewEffect)
{
    // Check if effect already exists
    for (FStatusEffect& Effect : ActiveEffects)
    {
        if (Effect.EffectName == NewEffect.EffectName)
        {
            // Refresh duration
            Effect.Duration = FMath::Max(Effect.Duration, NewEffect.Duration);
            return;
        }
    }
    
    // Add new effect
    ActiveEffects.Add(NewEffect);
}

void UStatusEffectComponent::RemoveStatusEffect(FName EffectName)
{
    ActiveEffects.RemoveAll([EffectName](const FStatusEffect& Effect)
    {
        return Effect.EffectName == EffectName;
    });
}

bool UStatusEffectComponent::HasStatusEffect(FName EffectName) const
{
    return ActiveEffects.ContainsByPredicate([EffectName](const FStatusEffect& Effect)
    {
        return Effect.EffectName == EffectName;
    });
}