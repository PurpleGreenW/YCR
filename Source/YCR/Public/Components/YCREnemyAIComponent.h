#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "YCREnemyAIComponent.generated.h"

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class YCR_API UYCREnemyAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UYCREnemyAIComponent();

protected:
	// Component lifecycle
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// AI Properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float MoveSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float AttackRange = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float ContactDamage = 10.0f;

private:
	UPROPERTY()
	class ACharacterBase* OwnerCharacter;

	UPROPERTY()
	class ACharacterPlayer* TargetPlayer;

	// AI behavior methods
	void MoveTowardsPlayer(float DeltaTime);
	void CheckAttackRange();
	void FindTargetPlayer();
	void ApplyContactDamage();
};