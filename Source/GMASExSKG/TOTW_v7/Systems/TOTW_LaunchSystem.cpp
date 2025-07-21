#include "TOTW_LaunchSystem.h"
#include "GMCE_MotionWarpingComponent.h"
#include "GMCE_OrganicMovementCmp.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"

ATOTW_LaunchSystem::ATOTW_LaunchSystem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATOTW_LaunchSystem::BeginPlay()
{
	Super::BeginPlay();
}

void ATOTW_LaunchSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATOTW_LaunchSystem::LaunchCharacterToLocation(AActor* Character, const FVector& TargetLocation, float LaunchHeight)
{
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("TOTW_LaunchSystem: Character is null"));
		return;
	}

	// Check if character is already launching
	if (IsCharacterLaunching(Character))
	{
		UE_LOG(LogTemp, Warning, TEXT("TOTW_LaunchSystem: Character is already launching"));
		return;
	}

	// Get the motion warping component
	UGMCE_MotionWarpingComponent* MotionWarpingComp = GetMotionWarpingComponent(Character);
	if (!MotionWarpingComp)
	{
		UE_LOG(LogTemp, Error, TEXT("TOTW_LaunchSystem: Character does not have motion warping component"));
		return;
	}

	// Setup motion warping target
	SetupMotionWarping(Character, TargetLocation, LaunchHeight);

	// Play the launch montage
	UGMCE_OrganicMovementCmp* MovementComp = GetMovementComponent(Character);
	if (MovementComp && DefaultLaunchMontage)
	{
		// Add character to launching list
		LaunchingCharacters.AddUnique(Character);
		
		// Play the montage using the correct GMCExtended method
		MovementComp->PlayMontage_Blocking(MovementComp->GetSkeletalMeshReference(), MovementComp->MontageTracker, DefaultLaunchMontage, 0.0f, LaunchSpeed);
		
		// Trigger blueprint event
		OnLaunchStarted(Character, TargetLocation);
		
		UE_LOG(LogTemp, Log, TEXT("TOTW_LaunchSystem: Started launch for character %s to location %s"), 
			*Character->GetName(), *TargetLocation.ToString());
	}
}

void ATOTW_LaunchSystem::LaunchCharacterToActor(AActor* Character, AActor* TargetActor, float LaunchHeight)
{
	if (!TargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("TOTW_LaunchSystem: Target actor is null"));
		return;
	}

	LaunchCharacterToLocation(Character, TargetActor->GetActorLocation(), LaunchHeight);
}

void ATOTW_LaunchSystem::LaunchCharacterToComponent(AActor* Character, USceneComponent* TargetComponent, float LaunchHeight)
{
	if (!TargetComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("TOTW_LaunchSystem: Target component is null"));
		return;
	}

	LaunchCharacterToLocation(Character, TargetComponent->GetComponentLocation(), LaunchHeight);
}

void ATOTW_LaunchSystem::LaunchCharacterWithMontage(AActor* Character, UAnimMontage* LaunchMontage, const FVector& TargetLocation, float LaunchHeight)
{
	if (!LaunchMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("TOTW_LaunchSystem: Launch montage is null"));
		return;
	}

	// Store the original montage
	UAnimMontage* OriginalMontage = DefaultLaunchMontage;
	DefaultLaunchMontage = LaunchMontage;

	// Launch with the custom montage
	LaunchCharacterToLocation(Character, TargetLocation, LaunchHeight);

	// Restore the original montage
	DefaultLaunchMontage = OriginalMontage;
}

bool ATOTW_LaunchSystem::IsCharacterLaunching(AActor* Character) const
{
	return LaunchingCharacters.Contains(Character);
}

void ATOTW_LaunchSystem::CancelLaunch(AActor* Character)
{
	if (!Character)
		return;

	// Remove from launching list
	LaunchingCharacters.Remove(Character);

	// Stop any playing montage
	UGMCE_OrganicMovementCmp* MovementComp = GetMovementComponent(Character);
	if (MovementComp)
	{
		MovementComp->StopMontage(MovementComp->GetSkeletalMeshReference(), MovementComp->MontageTracker, 0.1f);
	}

	// Clear motion warping targets
	UGMCE_MotionWarpingComponent* MotionWarpingComp = GetMotionWarpingComponent(Character);
	if (MotionWarpingComp)
	{
		MotionWarpingComp->RemoveWarpTargetByName(LaunchTargetName);
	}

	// Trigger blueprint event
	OnLaunchCancelled(Character);

	UE_LOG(LogTemp, Log, TEXT("TOTW_LaunchSystem: Cancelled launch for character %s"), *Character->GetName());
}

void ATOTW_LaunchSystem::SetupMotionWarping(AActor* Character, const FVector& TargetLocation, float Height)
{
	UGMCE_MotionWarpingComponent* MotionWarpingComp = GetMotionWarpingComponent(Character);
	if (!MotionWarpingComp)
		return;

	// Calculate the final target position with height offset
	FVector FinalTargetLocation = TargetLocation;
	FinalTargetLocation.Z += Height;

	// Create motion warp target
	FGMCE_MotionWarpTarget LaunchTarget;
	LaunchTarget.Name = LaunchTargetName;
	LaunchTarget.Location = FinalTargetLocation;
	LaunchTarget.Rotation = (FinalTargetLocation - Character->GetActorLocation()).Rotation();
	LaunchTarget.bFollowComponent = false;

	// Add or update the warp target
	MotionWarpingComp->AddOrUpdateWarpTarget(LaunchTarget);

	UE_LOG(LogTemp, Log, TEXT("TOTW_LaunchSystem: Set motion warp target %s to location %s"), 
		*LaunchTargetName.ToString(), *FinalTargetLocation.ToString());
}

void ATOTW_LaunchSystem::CreateLaunchTrajectory(const FVector& StartLocation, const FVector& TargetLocation, float Height, TArray<FVector>& OutTrajectory)
{
	OutTrajectory.Empty();

	// Calculate trajectory points for arc motion
	const int32 NumPoints = 20;
	const FVector Direction = (TargetLocation - StartLocation).GetSafeNormal();
	const float Distance = FVector::Dist(StartLocation, TargetLocation);
	const float StepDistance = Distance / (NumPoints - 1);

	for (int32 i = 0; i < NumPoints; ++i)
	{
		float Progress = static_cast<float>(i) / (NumPoints - 1);
		FVector CurrentLocation = FMath::Lerp(StartLocation, TargetLocation, Progress);
		
		// Add arc height
		if (bUseArcTrajectory)
		{
			float ArcProgress = FMath::Sin(Progress * PI);
			CurrentLocation.Z += ArcProgress * ArcHeight;
		}
		
		OutTrajectory.Add(CurrentLocation);
	}
}

FVector ATOTW_LaunchSystem::CalculateLaunchVelocity(const FVector& StartLocation, const FVector& TargetLocation, float Height)
{
	// Simple physics-based launch velocity calculation
	const FVector Displacement = TargetLocation - StartLocation;
	const float Distance = Displacement.Size2D();
	const float Time = Distance / 1000.0f; // Approximate time based on distance
	
	FVector Velocity = Displacement / Time;
	Velocity.Z = Height / Time; // Initial upward velocity
	
	return Velocity;
}

UGMCE_MotionWarpingComponent* ATOTW_LaunchSystem::GetMotionWarpingComponent(AActor* Character)
{
	if (!Character)
		return nullptr;

	// Try to find the motion warping component
	UGMCE_MotionWarpingComponent* MotionWarpingComp = Character->FindComponentByClass<UGMCE_MotionWarpingComponent>();
	
	if (!MotionWarpingComp)
	{
		// If not found, try to get it from a child actor
		TArray<AActor*> AttachedActors;
		Character->GetAttachedActors(AttachedActors);
		
		for (AActor* AttachedActor : AttachedActors)
		{
			MotionWarpingComp = AttachedActor->FindComponentByClass<UGMCE_MotionWarpingComponent>();
			if (MotionWarpingComp)
				break;
		}
	}

	return MotionWarpingComp;
}

UGMCE_OrganicMovementCmp* ATOTW_LaunchSystem::GetMovementComponent(AActor* Character)
{
	if (!Character)
		return nullptr;

	// Try to find the organic movement component
	UGMCE_OrganicMovementCmp* MovementComp = Character->FindComponentByClass<UGMCE_OrganicMovementCmp>();
	
	if (!MovementComp)
	{
		// If not found, try to get it from a child actor
		TArray<AActor*> AttachedActors;
		Character->GetAttachedActors(AttachedActors);
		
		for (AActor* AttachedActor : AttachedActors)
		{
			MovementComp = AttachedActor->FindComponentByClass<UGMCE_OrganicMovementCmp>();
			if (MovementComp)
				break;
		}
	}

	return MovementComp;
} 