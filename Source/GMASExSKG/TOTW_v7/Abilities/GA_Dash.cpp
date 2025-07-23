#include "GA_Dash.h"
#include "TOTW_v7/Systems/TOTW_LaunchSystem.h"
#include "GMCAbilityComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

UGA_Dash::UGA_Dash()
{
	// Set default ability properties
	AbilityTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Dash"));
	
	// Set cooldown
	CooldownTime = DashCooldown;
	
	// Note: Cost is handled through AbilityCost property in GMAS
	// You can set up a stamina cost effect in the Blueprint or through AbilityCost
}

void UGA_Dash::BeginAbility()
{
	Super::BeginAbility();

	// Check if we can dash
	if (bIsDashing)
	{
		UE_LOG(LogTemp, Warning, TEXT("GA_Dash: Already dashing"));
		EndAbility();
		return;
	}

	// Get the owning character
	ACharacter* Character = Cast<ACharacter>(GetOwnerActor());
	if (!Character)
	{
		UE_LOG(LogTemp, Error, TEXT("GA_Dash: No owning character"));
		EndAbility();
		return;
	}

	// Spawn or get the launch system
	if (!LaunchSystem)
	{
		LaunchSystem = GetWorld()->SpawnActor<ATOTW_LaunchSystem>();
		if (!LaunchSystem)
		{
			UE_LOG(LogTemp, Error, TEXT("GA_Dash: Failed to spawn launch system"));
			EndAbility();
			return;
		}
	}

	// Configure the launch system (these are passed as parameters to launch functions)
	// Note: LaunchSpeed and other settings are handled internally by the launch system

	// Perform the dash
	PerformDash();
}

void UGA_Dash::EndAbility()
{
	// Cancel any ongoing dash
	if (bIsDashing && LaunchSystem)
	{
		LaunchSystem->CancelLaunch(GetOwnerActor());
	}

	bIsDashing = false;
	Super::EndAbility();
}

void UGA_Dash::PerformDash()
{
	ACharacter* Character = Cast<ACharacter>(GetOwnerActor());
	if (!Character || !LaunchSystem)
	{
		UE_LOG(LogTemp, Error, TEXT("GA_Dash: Cannot perform dash - missing character or launch system"));
		return;
	}

	// Calculate dash target
	FVector DashTarget = CalculateDashTarget();
	
	// Start the dash with custom height
	bIsDashing = true;
	LaunchSystem->LaunchCharacterToLocation(Character, DashTarget, DashHeight);
	
	UE_LOG(LogTemp, Log, TEXT("GA_Dash: Started dash to %s"), *DashTarget.ToString());
}

FVector UGA_Dash::CalculateDashTarget() const
{
	ACharacter* Character = Cast<ACharacter>(GetOwnerActor());
	if (!Character)
	{
		return FVector::ZeroVector;
	}

	// Get character's forward direction
	FVector ForwardDirection = Character->GetActorForwardVector();
	
	// Calculate target location
	FVector StartLocation = Character->GetActorLocation();
	FVector TargetLocation = StartLocation + (ForwardDirection * DashDistance);
	
	// Optional: Add some height for arc trajectory
	if (bUseArcTrajectory)
	{
		TargetLocation.Z += DashHeight;
	}
	
	return TargetLocation;
}

// Note: Launch completion/cancellation events are handled through Blueprint events
// in the launch system. Override OnLaunchCompleted and OnLaunchCancelled in the
// Blueprint version of this ability to handle dash completion. 