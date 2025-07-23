#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GA_DashBPLibrary.generated.h"

class UGA_Dash;
class ACharacter;

/**
 * @class UGA_DashBPLibrary
 * @brief Blueprint function library for dash ability operations
 * 
 * Provides easy-to-use Blueprint functions for creating and managing dash abilities.
 */
UCLASS()
class GMASEXSKG_API UGA_DashBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// Create and grant dash ability
	UFUNCTION(BlueprintCallable, Category = "Dash Ability", meta = (WorldContext = "WorldContextObject"))
	static UGA_Dash* CreateDashAbility(const UObject* WorldContextObject, ACharacter* TargetCharacter, float Distance = 500.0f, float Height = 50.0f, float Speed = 1.0f);

	// Grant dash ability to character
	UFUNCTION(BlueprintCallable, Category = "Dash Ability")
	static bool GrantDashAbility(ACharacter* Character, UGA_Dash* DashAbility);

	// Activate dash ability
	UFUNCTION(BlueprintCallable, Category = "Dash Ability")
	static bool ActivateDashAbility(ACharacter* Character);

	// Check if character has dash ability
	UFUNCTION(BlueprintPure, Category = "Dash Ability")
	static bool HasDashAbility(ACharacter* Character);

	// Get dash ability from character
	UFUNCTION(BlueprintPure, Category = "Dash Ability")
	static UGA_Dash* GetDashAbility(ACharacter* Character);
}; 