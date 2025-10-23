#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "YCREnemyAIComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class YCR_API UYCREnemyAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UYCREnemyAIComponent();

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
							  FActorComponentTickFunction* ThisTickFunction) override;

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

	void MoveTowardsPlayer(float DeltaTime);
	void CheckAttackRange();
};