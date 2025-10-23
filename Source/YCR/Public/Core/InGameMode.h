// D:\Unreal Projects\YCR\Source\YCR\Public\Core\InGameMode.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "YCR/Public/Interfaces/ISpawnableInterface.h"
#include "InGameMode.generated.h"

// Forward declarations
class ACharacterPlayer;
class AEnemyBase;
class UYCRSpawnManager;
class UYCRWaveManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRunTimeUpdated, float, CurrentRunTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossSpawned, AActor*, BossActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRunCompleted);

/**
 * Main Game Mode for YCR in-game sessions
 * Handles enemy spawning, wave progression, and run management
 */
UCLASS()
class YCR_API AInGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AInGameMode();

    // =====================================================
    // Core Game Mode Functions
    // =====================================================
    
    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // =====================================================
    // Run Management
    // =====================================================
    
    /** Start a new run with specified parameters */
    UFUNCTION(BlueprintCallable, Category = "YCR|GameMode")
    void StartRun();
    
    /** End the current run */
    UFUNCTION(BlueprintCallable, Category = "YCR|GameMode")
    void EndRun(bool bVictory);
    
    /** Get current run time */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "YCR|GameMode")
    float GetCurrentRunTime() const { return CurrentRunTime; }
    
    /** Check if run time limit reached */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "YCR|GameMode")
    bool IsTimeLimitReached() const { return CurrentRunTime >= RunTimeLimit; }

    // =====================================================
    // Enemy Management
    // =====================================================
    
    /** Register enemy death for statistics */
    UFUNCTION(BlueprintCallable, Category = "YCR|GameMode")
    void OnEnemyKilled(AEnemyBase* KilledEnemy);
    
    /** Get current enemy count */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "YCR|GameMode")
    int32 GetCurrentEnemyCount() const { return CurrentEnemyCount; }

    // =====================================================
    // Boss Management
    // =====================================================
    
    /** Spawn the boss for current level */
    UFUNCTION(BlueprintCallable, Category = "YCR|GameMode")
    void SpawnBoss();
    
    /** Called when boss is defeated */
    UFUNCTION(BlueprintCallable, Category = "YCR|GameMode")
    void OnBossDefeated();

    // =====================================================
    // Events
    // =====================================================
    
    /** Called every second with current run time */
    UPROPERTY(BlueprintAssignable, Category = "YCR|Events")
    FOnRunTimeUpdated OnRunTimeUpdated;
    
    /** Called when boss spawns */
    UPROPERTY(BlueprintAssignable, Category = "YCR|Events")
    FOnBossSpawned OnBossSpawned;
    
    /** Called when run is completed */
    UPROPERTY(BlueprintAssignable, Category = "YCR|Events")
    FOnRunCompleted OnRunCompleted;

protected:
    // =====================================================
    // Configuration
    // =====================================================
    
    /** Time limit per level in seconds (10 minutes) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YCR|Config")
    float RunTimeLimit = 600.0f;
    
    /** Time when boss spawns (9 minutes) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YCR|Config")
    float BossSpawnTime = 540.0f;
    
    /** Maximum enemies allowed at once */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YCR|Config")
    int32 MaxEnemyCount = 100;
    
    /** Boss class to spawn */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YCR|Config")
    TSubclassOf<AEnemyBase> BossClass;

    // =====================================================
    // Components
    // =====================================================
    
    /** Spawn manager component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "YCR|Components")
    UYCRSpawnManager* SpawnManager;
    
    /** Wave manager component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "YCR|Components")
    UYCRWaveManager* WaveManager;

private:
    // =====================================================
    // Runtime State
    // =====================================================
    
    float CurrentRunTime = 0.0f;
    float LastRunTimeUpdate = 0.0f;
    int32 CurrentEnemyCount = 0;
    int32 TotalEnemiesKilled = 0;
    bool bBossSpawned = false;
    bool bRunActive = false;
    
    /** Handle death swarm when time limit reached */
    void StartDeathSwarm();
    
    /** Update run statistics */
    void UpdateRunStatistics(float DeltaTime);
    
    /** Check and trigger run events */
    void CheckRunEvents();
};