// D:\Unreal Projects\YCR\Source\YCR\Private\Core\InGameMode.cpp

#include "YCR/Public/Core/InGameMode.h"
#include "YCR/Public/Core/InGameState.h"
#include "YCR/Public/Core/GameInstanceYCR.h"
#include "YCR/Public/Character/CharacterPlayer.h"
#include "YCR/Public/Enemies/EnemyBase.h"
#include "YCR/Public/Systems/YCRSpawnManager.h"
#include "YCR/Public/Systems/YCRWaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AInGameMode::AInGameMode()
{
    // Set default pawn class
    DefaultPawnClass = ACharacterPlayer::StaticClass();
    
    // Set default game state
    GameStateClass = AInGameState::StaticClass();
    
    // Enable ticking
    PrimaryActorTick.bCanEverTick = true;
    
    // Create components
    SpawnManager = CreateDefaultSubobject<UYCRSpawnManager>(TEXT("SpawnManager"));
    WaveManager = CreateDefaultSubobject<UYCRWaveManager>(TEXT("WaveManager"));
}

void AInGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);
    
    UE_LOG(LogTemp, Log, TEXT("YCR InGameMode initialized for map: %s"), *MapName);
}

void AInGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // Auto-start run after short delay
    FTimerHandle StartDelayHandle;
    GetWorldTimerManager().SetTimer(StartDelayHandle, this, &AInGameMode::StartRun, 2.0f, false);
}

void AInGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (bRunActive)
    {
        UpdateRunStatistics(DeltaTime);
        CheckRunEvents();
    }
}

void AInGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (bRunActive)
    {
        EndRun(false);
    }
    
    Super::EndPlay(EndPlayReason);
}

void AInGameMode::StartRun()
{
    UE_LOG(LogTemp, Log, TEXT("Starting new YCR run"));
    
    // Reset run state
    CurrentRunTime = 0.0f;
    LastRunTimeUpdate = 0.0f;
    TotalEnemiesKilled = 0;
    CurrentEnemyCount = 0;
    bBossSpawned = false;
    bRunActive = true;
    
    // Get game instance for run data
    if (UGameInstanceYCR* GameInstance = Cast<UGameInstanceYCR>(GetGameInstance()))
    {
        GameInstance->StartNewRun();
    }
    
    // Start spawning
    if (SpawnManager)
    {
        SpawnManager->StartSpawning();
    }
    
    if (WaveManager)
    {
        WaveManager->StartWaveProgression();
    }
}

void AInGameMode::EndRun(bool bVictory)
{
    if (!bRunActive)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Ending run - Victory: %s"), bVictory ? TEXT("Yes") : TEXT("No"));
    
    bRunActive = false;
    
    // Stop spawning
    if (SpawnManager)
    {
        SpawnManager->StopSpawning();
    }
    
    if (WaveManager)
    {
        WaveManager->StopWaveProgression();
    }
    
    // Update game instance
    if (UGameInstanceYCR* GameInstance = Cast<UGameInstanceYCR>(GetGameInstance()))
    {
        GameInstance->EndCurrentRun(bVictory);
    }
    
    // Broadcast completion
    OnRunCompleted.Broadcast();
    
    // Handle transition
    if (bVictory)
    {
        // Open portal to next level
        // TODO: Implement portal spawning
    }
    else
    {
        // Return to menu after delay
        FTimerHandle ReturnHandle;
        GetWorldTimerManager().SetTimer(ReturnHandle, [this]()
        {
            UGameplayStatics::OpenLevel(this, "MainMenu");
        }, 3.0f, false);
    }
}

void AInGameMode::OnEnemyKilled(AEnemyBase* KilledEnemy)
{
    if (!KilledEnemy)
    {
        return;
    }
    
    TotalEnemiesKilled++;
    CurrentEnemyCount = FMath::Max(0, CurrentEnemyCount - 1);
    
    // Update game instance
    if (UGameInstanceYCR* GameInstance = Cast<UGameInstanceYCR>(GetGameInstance()))
    {
        GameInstance->AddKillCount(1);
        
        // Check if it was a boss
        if (KilledEnemy->GetClass()->IsChildOf(BossClass))
        {
            OnBossDefeated();
        }
    }
}

void AInGameMode::SpawnBoss()
{
    if (bBossSpawned || !BossClass)
    {
        return;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Boss spawn time reached! Spawning boss..."));
    
    bBossSpawned = true;
    
    // Find spawn location
    FVector SpawnLocation = FVector::ZeroVector;
    if (ACharacterPlayer* Player = Cast<ACharacterPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0)))
    {
        // Spawn boss at distance from player
        FVector Direction = FMath::VRand();
        Direction.Z = 0;
        Direction.Normalize();
        SpawnLocation = Player->GetActorLocation() + (Direction * 2000.0f);
        SpawnLocation.Z = Player->GetActorLocation().Z;
    }
    
    // Spawn boss
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    
    if (AActor* Boss = GetWorld()->SpawnActor<AActor>(BossClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams))
    {
        OnBossSpawned.Broadcast(Boss);
    }
}

void AInGameMode::OnBossDefeated()
{
    UE_LOG(LogTemp, Log, TEXT("Boss defeated! Victory!"));
    EndRun(true);
}

void AInGameMode::StartDeathSwarm()
{
    UE_LOG(LogTemp, Warning, TEXT("Time limit reached! Starting death swarm..."));
    
    // Dramatically increase spawn rates
    if (SpawnManager)
    {
        SpawnManager->StartDeathSwarm();
    }
}

void AInGameMode::UpdateRunStatistics(float DeltaTime)
{
    CurrentRunTime += DeltaTime;
    
    // Update every second
    if (CurrentRunTime - LastRunTimeUpdate >= 1.0f)
    {
        LastRunTimeUpdate = CurrentRunTime;
        OnRunTimeUpdated.Broadcast(CurrentRunTime);
        
        // Update game instance
        if (UGameInstanceYCR* GameInstance = Cast<UGameInstanceYCR>(GetGameInstance()))
        {
            GameInstance->SetRunTime(CurrentRunTime);
        }
    }
}

void AInGameMode::CheckRunEvents()
{
    // Check boss spawn time
    if (!bBossSpawned && CurrentRunTime >= BossSpawnTime)
    {
        SpawnBoss();
    }
    
    // Check time limit for death swarm
    if (CurrentRunTime >= RunTimeLimit)
    {
        StartDeathSwarm();
    }
}