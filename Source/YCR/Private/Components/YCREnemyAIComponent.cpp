#include "Components/YCREnemyAIComponent.h"
#include "Character/CharacterBase.h"
#include "Character/CharacterPlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UYCREnemyAIComponent::UYCREnemyAIComponent()
{
	// Set this component to be ticked every frame
	PrimaryComponentTick.bCanEverTick = true;
	
	// Initialize pointers
	OwnerCharacter = nullptr;
	TargetPlayer = nullptr;
}

// Called when the game starts
void UYCREnemyAIComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Get the owner character
	OwnerCharacter = Cast<ACharacterBase>(GetOwner());
	
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("YCREnemyAIComponent: Owner is not a CharacterBase!"));
		return;
	}
	
	// Find the player character
	FindTargetPlayer();
}

// Called every frame
void UYCREnemyAIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Ensure we have valid references
	if (!OwnerCharacter || !OwnerCharacter->IsAlive())
	{
		return;
	}
	
	// Try to find player if we don't have one
	if (!TargetPlayer || !IsValid(TargetPlayer))
	{
		FindTargetPlayer();
		return;
	}
	
	// Move towards player
	MoveTowardsPlayer(DeltaTime);
	
	// Check if we're in attack range
	CheckAttackRange();
}

void UYCREnemyAIComponent::MoveTowardsPlayer(float DeltaTime)
{
	if (!OwnerCharacter || !TargetPlayer || !TargetPlayer->IsAlive())
	{
		return;
	}
	
	// Get current locations
	FVector OwnerLocation = OwnerCharacter->GetActorLocation();
	FVector TargetLocation = TargetPlayer->GetActorLocation();
	
	// Calculate direction to player
	FVector DirectionToPlayer = (TargetLocation - OwnerLocation);
	DirectionToPlayer.Z = 0.0f; // Keep movement on horizontal plane
	DirectionToPlayer.Normalize();
	
	// Calculate movement distance for this frame
	float MovementDistance = MoveSpeed * DeltaTime;
	
	// Move the character
	if (UCharacterMovementComponent* MovementComponent = OwnerCharacter->GetCharacterMovement())
	{
		// Use AddMovementInput for proper character movement
		OwnerCharacter->AddMovementInput(DirectionToPlayer, MovementDistance);
		
		// Make the character face the player
		FRotator NewRotation = DirectionToPlayer.Rotation();
		NewRotation.Pitch = 0.0f;
		NewRotation.Roll = 0.0f;
		OwnerCharacter->SetActorRotation(NewRotation);
	}
	else
	{
		// Fallback: Direct position update (not recommended for characters)
		FVector NewLocation = OwnerLocation + (DirectionToPlayer * MovementDistance);
		OwnerCharacter->SetActorLocation(NewLocation, true);
	}
	
	// Debug visualization
#if WITH_EDITOR
	if (GEngine && GEngine->GetDebugLocalPlayer())
	{
		DrawDebugLine(GetWorld(), OwnerLocation, TargetLocation, FColor::Red, false, -1.0f, 0, 2.0f);
	}
#endif
}

void UYCREnemyAIComponent::CheckAttackRange()
{
	if (!OwnerCharacter || !TargetPlayer || !TargetPlayer->IsAlive())
	{
		return;
	}
	
	float DistanceToPlayer = FVector::Dist(OwnerCharacter->GetActorLocation(), TargetPlayer->GetActorLocation());
	
	if (DistanceToPlayer <= AttackRange)
	{
		// In attack range - apply contact damage
		ApplyContactDamage();
	}
}

void UYCREnemyAIComponent::FindTargetPlayer()
{
	if (!GetWorld())
	{
		return;
	}
	
	// Try to find the player character in the world
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterPlayer::StaticClass(), FoundActors);
	
	if (FoundActors.Num() > 0)
	{
		TargetPlayer = Cast<ACharacterPlayer>(FoundActors[0]);
		
		if (TargetPlayer)
		{
			UE_LOG(LogTemp, Log, TEXT("YCREnemyAIComponent: Found target player"));
		}
	}
	else
	{
		// Alternative: Try to get player controller's pawn
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController && PlayerController->GetPawn())
		{
			TargetPlayer = Cast<ACharacterPlayer>(PlayerController->GetPawn());
		}
	}
}

void UYCREnemyAIComponent::ApplyContactDamage()
{
	if (!TargetPlayer || !OwnerCharacter)
	{
		return;
	}
	
	// Apply damage to the player
	// This is a simple implementation - you may want to use GAS (Gameplay Ability System) instead
	UGameplayStatics::ApplyPointDamage(
		TargetPlayer,
		ContactDamage,
		OwnerCharacter->GetActorLocation(),
		nullptr,
		OwnerCharacter->GetInstigatorController(),
		OwnerCharacter,
		UDamageType::StaticClass()
	);
	
	// Log for debugging
	UE_LOG(LogTemp, Log, TEXT("YCREnemyAIComponent: Applied %f contact damage to player"), ContactDamage);
}