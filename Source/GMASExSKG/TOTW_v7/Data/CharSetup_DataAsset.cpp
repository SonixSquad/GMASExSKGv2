#include "CharSetup_DataAsset.h"
#include "TOTW_v7/Components/TOTW_CustomMovement.h"
#include "TOTW_v7/Actors/TOTW_MWPawn_Base.h"
#include "Components/SkeletalMeshComponent.h"
#include "GMCAbilityComponent.h"
#include "GMCAbilitySystem.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

UCharSetup_DataAsset::UCharSetup_DataAsset()
{
	CharacterDisplayName = FText::FromString(TEXT("New Character"));
	CharacterDescription = FText::FromString(TEXT("Character setup description"));
	CharacterID = TEXT("NewCharacter");
	Faction = ECharacterFaction::Neutral;
	
	// Default to advanced movement component (includes all Blueprint functionality)
	ComponentSetup.MovementComponentClass = UTOTW_CustomMovement::StaticClass();
	
	// Initialize with empty input mapping contexts array
	InputMappingContexts.Empty();
}

#if WITH_EDITOR
EDataValidationResult UCharSetup_DataAsset::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = Super::IsDataValid(ValidationErrors);

	// Validate character ID is not empty
	if (CharacterID.IsNone() || CharacterID.ToString().IsEmpty())
	{
		ValidationErrors.Add(FText::FromString(TEXT("Character ID cannot be empty")));
		Result = EDataValidationResult::Invalid;
	}

	// Validate character mesh is set
	if (CharacterMesh.IsNull())
	{
		ValidationErrors.Add(FText::FromString(TEXT("Character Mesh must be set")));
		Result = EDataValidationResult::Invalid;
	}

	// Validate abilities have valid tags
	for (int32 i = 0; i < StartingAbilities.Num(); i++)
	{
		const FCharacterAbilitySetup& AbilitySetup = StartingAbilities[i];
		if (AbilitySetup.AbilityTag.IsValid() == false)
		{
			ValidationErrors.Add(FText::FromString(FString::Printf(TEXT("Starting Ability at index %d has invalid Ability Tag"), i)));
			Result = EDataValidationResult::Invalid;
		}
	}


	// Validate input mapping contexts
	for (int32 i = 0; i < InputMappingContexts.Num(); i++)
	{
		const FInputMappingContextSetup& ContextSetup = InputMappingContexts[i];
		
		if (ContextSetup.InputMappingContext.IsNull())
		{
			ValidationErrors.Add(FText::FromString(FString::Printf(TEXT("Input Mapping Context at index %d is null"), i)));
			Result = EDataValidationResult::Invalid;
		}
		
		if (ContextSetup.ContextName.IsEmpty())
		{
			ValidationErrors.Add(FText::FromString(FString::Printf(TEXT("Input Mapping Context at index %d has empty context name"), i)));
			Result = EDataValidationResult::Invalid;
		}
		
		if (ContextSetup.Priority < 0)
		{
			ValidationErrors.Add(FText::FromString(FString::Printf(TEXT("Input Mapping Context '%s' has negative priority (%d)"), *ContextSetup.ContextName, ContextSetup.Priority)));
			Result = EDataValidationResult::Invalid;
		}
	}

	return Result;
}
#endif

FPrimaryAssetId UCharSetup_DataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("CharacterSetup"), CharacterID);
}

void UCharSetup_DataAsset::ApplyToPawn(ATOTW_MWPawn_Base* TargetPawn) const
{
	if (!TargetPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyToPawn: TargetPawn is null"));
		return;
	}

	// Apply all properties
	ApplySkeletalMesh(TargetPawn);
	ApplyAnimationBlueprint(TargetPawn);
	GrantStartingAbilities(TargetPawn);
}

void UCharSetup_DataAsset::ApplySkeletalMesh(ATOTW_MWPawn_Base* TargetPawn) const
{
	if (!TargetPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplySkeletalMesh: TargetPawn is null"));
		return;
	}

	// Load and apply skeletal mesh directly
	if (!CharacterMesh.IsNull())
	{
		// Check if already loaded
		USkeletalMesh* LoadedMesh = CharacterMesh.Get();
		if (!LoadedMesh)
		{
			// Load synchronously if not already loaded
			LoadedMesh = CharacterMesh.LoadSynchronous();
		}
		
		if (LoadedMesh)
		{
			// Get the skeletal mesh component and set the mesh directly
			USkeletalMeshComponent* MeshComponent = TargetPawn->GetComponentByClass<USkeletalMeshComponent>();
			if (MeshComponent)
			{
				MeshComponent->SetSkeletalMesh(LoadedMesh);
				UE_LOG(LogTemp, Log, TEXT("Applied skeletal mesh: %s"), *LoadedMesh->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No skeletal mesh component found on pawn"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load skeletal mesh: %s"), *CharacterMesh.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Character mesh is null in data asset"));
	}
}

void UCharSetup_DataAsset::ApplyAnimationBlueprint(ATOTW_MWPawn_Base* TargetPawn) const
{
	if (!TargetPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyAnimationBlueprint: TargetPawn is null"));
		return;
	}

	USkeletalMeshComponent* MeshComponent = TargetPawn->GetComponentByClass<USkeletalMeshComponent>();
	if (!MeshComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyAnimationBlueprint: No skeletal mesh component found on pawn"));
		return;
	}

	// Apply animation blueprint
	if (AnimationBlueprint)
	{
		MeshComponent->SetAnimInstanceClass(AnimationBlueprint);
		UE_LOG(LogTemp, Log, TEXT("Applied animation blueprint: %s"), *AnimationBlueprint->GetName());
	}
}

void UCharSetup_DataAsset::GrantStartingAbilities(ATOTW_MWPawn_Base* TargetPawn) const
{
	if (!TargetPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("GrantStartingAbilities: TargetPawn is null"));
		return;
	}

	UGMC_AbilitySystemComponent* AbilityComponent = TargetPawn->GetComponentByClass<UGMC_AbilitySystemComponent>();
	if (!AbilityComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("GrantStartingAbilities: No ability system component found on pawn"));
		return;
	}

	// Grant each starting ability by tag
	for (const FCharacterAbilitySetup& AbilitySetup : StartingAbilities)
	{
		if (AbilitySetup.AbilityTag.IsValid())
		{
			AbilityComponent->GrantAbilityByTag(AbilitySetup.AbilityTag);
			UE_LOG(LogTemp, Log, TEXT("Granted ability by tag: %s"), *AbilitySetup.AbilityTag.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid ability tag in starting abilities"));
		}
	}
} 