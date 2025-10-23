// InteractableInterface.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteractableInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UIInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for all interactable objects in YCR
 * Used by: Chests, NPCs, Shrines, Vendors
 */
class YCR_API IIInteractableInterface
{
	GENERATED_BODY()

public:
	// Check if this object can be interacted with
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Interaction")
	bool CanInteract(ACharacter* InteractingCharacter) const;
    
	// Execute the interaction
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Interaction")
	void Interact(ACharacter* InteractingCharacter);
    
	// Get the interaction prompt text
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "YCR|Interaction")
	FText GetInteractionPrompt() const;
};