// IDamageableInterface.h
// Copyright YCR Project

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "YCR/Public/Enums/EYCRElements.h"
#include "IDamageableInterface.generated.h"

// Forward declarations
struct FDamageEvent;
class AController;
class AActor;

UINTERFACE(MinimalAPI, Blueprintable)
class UDamageableInterface : public UInterface
{
    GENERATED_BODY()
};

class YCR_API IDamageableInterface
{
    GENERATED_BODY()

public:
    /**
     * Apply damage to this entity
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Damage")
    void ReceiveDamage(float DamageAmount, const FDamageEvent& DamageEvent, 
                      AController* EventInstigator, AActor* DamageCauser);

    /**
     * Apply elemental damage with type effectiveness
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Damage")
    float ReceiveElementalDamage(float DamageAmount, EYCRElementType DamageElement,
                                AController* EventInstigator, AActor* DamageCauser);

    /**
     * Check if this entity is currently dead
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Damage")
    bool IsDead() const;

    /**
     * Get the current health percentage
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Damage")
    float GetHealthPercent() const;

    /**
     * Get the maximum health of this entity
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Damage")
    float GetMaxHealth() const;

    /**
     * Get the current health of this entity
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Damage")
    float GetCurrentHealth() const;

    /**
     * Get the element type of this entity
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Damage")
    EYCRElementType GetElementType() const;

    /**
     * Can this entity be damaged?
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Damage")
    bool CanBeDamaged() const;

    /**
     * Called when this entity dies
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Damage")
    void OnDeath(AController* KillerController, AActor* KillerActor);

    /**
     * Event called after damage is applied (already BlueprintImplementableEvent - keep as is)
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "YCR|Damage")
    void OnDamageReceived(float DamageDealt, EYCRElementType DamageElement,
                         AController* EventInstigator, AActor* DamageCauser);

    /**
     * Event called when health reaches zero (already BlueprintImplementableEvent - keep as is)
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "YCR|Damage")
    void OnDeathEvent(AController* KillerController, AActor* KillerActor);
};