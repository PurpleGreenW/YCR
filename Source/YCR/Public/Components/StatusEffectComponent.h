#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "YCR/Public/Enums/EElementTypes.h"
#include "StatusEffectComponent.generated.h"

USTRUCT(BlueprintType)
struct FStatusEffect
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FName EffectName;
    
	UPROPERTY(BlueprintReadWrite)
	float Duration;
    
	UPROPERTY(BlueprintReadWrite)
	float TickDamage;
    
	UPROPERTY(BlueprintReadWrite)
	float SlowPercent;
    
	UPROPERTY(BlueprintReadWrite)
	EElementTypes ElementType = EElementTypes::Neutral;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class YCR_API UStatusEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatusEffectComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TArray<FStatusEffect> ActiveEffects;
    
	void ProcessStatusEffects(float DeltaTime);

public:
	UFUNCTION(BlueprintCallable, Category = "YCR|StatusEffects")
	void ApplyStatusEffect(const FStatusEffect& NewEffect);
    
	UFUNCTION(BlueprintCallable, Category = "YCR|StatusEffects")
	void RemoveStatusEffect(FName EffectName);
    
	UFUNCTION(BlueprintPure, Category = "YCR|StatusEffects")
	bool HasStatusEffect(FName EffectName) const;
};