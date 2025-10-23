#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "YCRSpawnManager.generated.h"

USTRUCT(BlueprintType)
struct FSpawnWaveData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StartTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<class ACharacterBase>> MonsterClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SpawnCount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnRate = 1.0f; // Monsters per second

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HealthMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageMultiplier = 1.0f;
};

UCLASS()
class YCR_API AYCRSpawnManager : public AActor
{
	GENERATED_BODY()

public:
	AYCRSpawnManager();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* SpawnWaveTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	float SpawnRadius = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	float MinSpawnDistance = 500.0f;

private:
	float GameTime = 0.0f;
	int32 CurrentWaveIndex = 0;
    
	void UpdateSpawning(float DeltaTime);
	FVector GetRandomSpawnLocation() const;
};