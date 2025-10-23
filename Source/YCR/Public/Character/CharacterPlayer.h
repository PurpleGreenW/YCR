// Copyright YCR Project

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "InputActionValue.h"
#include "CharacterPlayer.generated.h"

// Forward declarations
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UYCREnemyAIComponent;
struct FInputActionValue;

/**
 * Player character class for YCR
 * Handles player input, camera, and player-specific abilities
 */
UCLASS()
class YCR_API ACharacterPlayer : public ACharacterBase
{
    GENERATED_BODY()

public:
    ACharacterPlayer();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // =====================================================
    // Components
    // =====================================================
    
    /** Camera boom positioning the camera behind the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "YCR|Camera", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    /** Follow camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "YCR|Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

    // =====================================================
    // Input
    // =====================================================
    
    /** MappingContext for player input */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "YCR|Input", meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* DefaultMappingContext;

    /** Jump Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "YCR|Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* JumpAction;

    /** Move Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "YCR|Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* MoveAction;

    /** Look Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "YCR|Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* LookAction;

    /** Interact Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "YCR|Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* InteractAction;

    // =====================================================
    // Player Stats
    // =====================================================
    
    /** Experience points for current level */
    UPROPERTY(BlueprintReadOnly, Category = "YCR|Stats")
    float CurrentExperience;

    /** Experience needed for next level */
    UPROPERTY(BlueprintReadOnly, Category = "YCR|Stats")
    float ExperienceToNextLevel;

    /** Pickup radius for collecting items/exp */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "YCR|Stats", meta = (ClampMin = "100.0", ClampMax = "1000.0"))
    float PickupRadius;

    /** Gold collected in current run */
    UPROPERTY(BlueprintReadOnly, Category = "YCR|Currency")
    int32 CurrentGold;

    /** Silver/Essence for meta progression */
    UPROPERTY(BlueprintReadOnly, Category = "YCR|Currency")
    int32 CurrentEssence;

private:
    // =====================================================
    // Input Callbacks
    // =====================================================
    
    /** Called for movement input */
    void Move(const FInputActionValue& Value);

    /** Called for looking input */
    void Look(const FInputActionValue& Value);

    /** Called for interact input */
    void Interact();

    // =====================================================
    // Gameplay Functions
    // =====================================================
    
    /** Handle collecting experience */
    void CollectExperience(float Amount);

    /** Level up the character */
    void LevelUp();

    /** Check for nearby interactables */
    void CheckForInteractables();

    /** Collect items in pickup radius */
    void CollectNearbyItems();

public:
    // =====================================================
    // Public Interface
    // =====================================================
    
    /** Add experience to player */
    UFUNCTION(BlueprintCallable, Category = "YCR|Experience")
    void AddExperience(float Amount);

    /** Add gold to player */
    UFUNCTION(BlueprintCallable, Category = "YCR|Currency")
    void AddGold(int32 Amount);

    /** Add essence to player */
    UFUNCTION(BlueprintCallable, Category = "YCR|Currency")
    void AddEssence(int32 Amount);

    /** Get current experience percentage */
    UFUNCTION(BlueprintCallable, Category = "YCR|Experience")
    float GetExperiencePercent() const;

    /** Get current pickup radius */
    UFUNCTION(BlueprintCallable, Category = "YCR|Stats")
    float GetPickupRadius() const { return PickupRadius; }

    /** Called when player levels up - Blueprint implementable */
    UFUNCTION(BlueprintImplementableEvent, Category = "YCR|Experience")
    void OnLevelUp();

    /** Called when player collects item - Blueprint implementable */
    UFUNCTION(BlueprintImplementableEvent, Category = "YCR|Pickup")
    void OnItemCollected(const FName& ItemName, int32 Quantity);

protected:
    // =====================================================
    // Overrides from CharacterBase
    // =====================================================
    
    /** Initialize ability system with player-specific abilities */
    virtual void InitializeAbilitySystem() override;

    /** Grant player-specific default abilities */
    virtual void GrantDefaultAbilities() override;

    /** Called when a gameplay effect is applied to self */
    void OnGameplayEffectApplied(UAbilitySystemComponent* Source, 
        const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveHandle);

private:
    /** Timer handle for periodic item collection */
    FTimerHandle ItemCollectionTimerHandle;

    /** Cached reference to player controller */
    UPROPERTY()
    class APlayerController* YCRPlayerController;
};