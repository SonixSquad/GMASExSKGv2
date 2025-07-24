#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Engine/SkeletalMesh.h"
#include "CharSetup_DataAsset.generated.h"

// Forward declarations
class USkeletalMesh;
class UAnimBlueprint;
class UInputMappingContext;
class UTOTW_CustomMovement;



UENUM(BlueprintType)
enum class ECharacterFaction : uint8
{
	Neutral		UMETA(DisplayName = "Neutral"),
	Faction_A		UMETA(DisplayName = "Faction_A"),
	Faction_B	UMETA(DisplayName = "Faction_B")
};

USTRUCT(BlueprintType)
struct FCharacterAbilitySetup
{
	GENERATED_BODY()

	/** The gameplay ability tag to grant */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FGameplayTag AbilityTag;

	/** Input tag for this ability (if any) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FGameplayTag InputTag;

	/** Whether this ability should be activated automatically on spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	bool bAutoActivate = false;

	FCharacterAbilitySetup()
	{
		bAutoActivate = false;
	}
};

USTRUCT(BlueprintType)
struct FCharacterComponentSetup
{
	GENERATED_BODY()

	/** Movement component class to use (defaults to basic TOTW_CustomMovement) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta=(AllowAbstract="false", BlueprintBaseOnly="true"))
	TSubclassOf<UTOTW_CustomMovement> MovementComponentClass;

	// TODO: Add additional component setup properties as needed
	// int32 AdditionalComponents;

};

USTRUCT(BlueprintType)
struct FCharacterCameraSetup
{
	GENERATED_BODY()

};

USTRUCT(BlueprintType)
struct FInputMappingContextSetup
{
	GENERATED_BODY()

	/** The input mapping context to add */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;

	/** Priority for this input mapping context (higher values = higher priority) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (ClampMin = 0))
	int32 Priority = 0;

	/** Optional name/description for this input context (for debugging/organization) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	FString ContextName;

	/** Whether this context should be added automatically on character spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool bAutoAdd = true;

	FInputMappingContextSetup()
	{
		Priority = 0;
		ContextName = TEXT("");
		bAutoAdd = true;
	}
};

/**
 * Data asset that defines a complete character setup template.
 * This allows designers to create reusable character configurations
 * without needing to create separate Blueprint classes.
 */
UCLASS(BlueprintType)
class GMASEXSKG_API UCharSetup_DataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UCharSetup_DataAsset();

	/** Display name for this character setup */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Info")
	FText CharacterDisplayName;

	/** Description of this character setup */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Info", meta = (MultiLine = true))
	FText CharacterDescription;

	/** Unique identifier for this character setup */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Info")
	FName CharacterID;

	/** Character faction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Info")
	ECharacterFaction Faction = ECharacterFaction::Neutral;

	/** Pawn class to use for this character setup */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Basic Info")
	TSubclassOf<class ATOTW_MWPawn_Base> CharacterClass;

	/** Skeletal mesh for this character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TSoftObjectPtr<USkeletalMesh> CharacterMesh;

	/** Animation Blueprint class for this character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TSubclassOf<UAnimInstance> AnimationBlueprint;

	/** Camera configuration */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FCharacterCameraSetup CameraSetup;

	/** Component configuration */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FCharacterComponentSetup ComponentSetup;

	/** Gameplay abilities to grant to this character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<FCharacterAbilitySetup> StartingAbilities;

	/** Gameplay tags to apply to this character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	FGameplayTagContainer CharacterTags;

	/** Input mapping contexts for this character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TArray<FInputMappingContextSetup> InputMappingContexts;

	/** Character-specific data (can be used for custom logic) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Data")
	TMap<FString, FString> CustomProperties;

#if WITH_EDITOR
	/** Validate the data asset setup */
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif

	/** Get the primary asset type for this data asset */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	// Helper functions to apply data asset to spawned actor
	UFUNCTION(BlueprintCallable, Category = "Character Setup")
	void ApplyToPawn(ATOTW_MWPawn_Base* TargetPawn) const;

	UFUNCTION(BlueprintCallable, Category = "Character Setup")
	void ApplySkeletalMesh(ATOTW_MWPawn_Base* TargetPawn) const;

	UFUNCTION(BlueprintCallable, Category = "Character Setup")
	void ApplyAnimationBlueprint(ATOTW_MWPawn_Base* TargetPawn) const;

	UFUNCTION(BlueprintCallable, Category = "Character Setup")
	void GrantStartingAbilities(ATOTW_MWPawn_Base* TargetPawn) const;
}; 
