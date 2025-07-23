#include "GA_DashBPLibrary.h"
#include "GA_Dash.h"
#include "GMCAbilityComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "GameplayTagContainer.h"

UGA_Dash* UGA_DashBPLibrary::CreateDashAbility(const UObject* WorldContextObject, ACharacter* TargetCharacter, float Distance, float Height, float Speed)
{
	if (!WorldContextObject || !TargetCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("GA_DashBPLibrary: Invalid parameters for CreateDashAbility"));
		return nullptr;
	}

	// Create the dash ability
	UGA_Dash* DashAbility = NewObject<UGA_Dash>(TargetCharacter);
	if (!DashAbility)
	{
		UE_LOG(LogTemp, Error, TEXT("GA_DashBPLibrary: Failed to create dash ability"));
		return nullptr;
	}

	// Configure the dash ability
	DashAbility->DashDistance = Distance;
	DashAbility->DashHeight = Height;
	DashAbility->DashSpeed = Speed;

	UE_LOG(LogTemp, Log, TEXT("GA_DashBPLibrary: Created dash ability with distance=%f, height=%f, speed=%f"), Distance, Height, Speed);

	return DashAbility;
}

bool UGA_DashBPLibrary::GrantDashAbility(ACharacter* Character, UGA_Dash* DashAbility)
{
	if (!Character || !DashAbility)
	{
		UE_LOG(LogTemp, Warning, TEXT("GA_DashBPLibrary: Invalid parameters for GrantDashAbility"));
		return false;
	}

	// Get the ability component
	UGMC_AbilitySystemComponent* AbilityComponent = Character->FindComponentByClass<UGMC_AbilitySystemComponent>();
	if (!AbilityComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("GA_DashBPLibrary: Character does not have ability component"));
		return false;
	}

	// Grant the ability by tag
	AbilityComponent->GrantAbilityByTag(DashAbility->AbilityTag);
	
	UE_LOG(LogTemp, Log, TEXT("GA_DashBPLibrary: Granted dash ability to %s"), *Character->GetName());
	
	return true;
}

bool UGA_DashBPLibrary::ActivateDashAbility(ACharacter* Character)
{
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("GA_DashBPLibrary: Invalid character for ActivateDashAbility"));
		return false;
	}

	// Get the ability component
	UGMC_AbilitySystemComponent* AbilityComponent = Character->FindComponentByClass<UGMC_AbilitySystemComponent>();
	if (!AbilityComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("GA_DashBPLibrary: Character does not have ability component"));
		return false;
	}

	// Check if the character has the dash ability
	FGameplayTag DashTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Dash"));
	if (!AbilityComponent->HasGrantedAbilityTag(DashTag))
	{
		UE_LOG(LogTemp, Warning, TEXT("GA_DashBPLibrary: Character does not have dash ability"));
		return false;
	}

	// Activate the ability using the component
	bool bSuccess = AbilityComponent->TryActivateAbility(UGA_Dash::StaticClass());
	
	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("GA_DashBPLibrary: Activated dash ability for %s"), *Character->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GA_DashBPLibrary: Failed to activate dash ability for %s"), *Character->GetName());
	}

	return bSuccess;
}

bool UGA_DashBPLibrary::HasDashAbility(ACharacter* Character)
{
	if (!Character)
	{
		return false;
	}

	UGA_Dash* DashAbility = GetDashAbility(Character);
	return DashAbility != nullptr;
}

UGA_Dash* UGA_DashBPLibrary::GetDashAbility(ACharacter* Character)
{
	if (!Character)
	{
		return nullptr;
	}

	// Get the ability component
	UGMC_AbilitySystemComponent* AbilityComponent = Character->FindComponentByClass<UGMC_AbilitySystemComponent>();
	if (!AbilityComponent)
	{
		return nullptr;
	}

	// Check if the dash ability is granted by tag
	FGameplayTag DashTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Dash"));
	if (AbilityComponent->HasGrantedAbilityTag(DashTag))
	{
		// Create a new instance of the dash ability
		UGA_Dash* DashAbility = NewObject<UGA_Dash>(AbilityComponent, UGA_Dash::StaticClass());
		return DashAbility;
	}

	return nullptr;
} 