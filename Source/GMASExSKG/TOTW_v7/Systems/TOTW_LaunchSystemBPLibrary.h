#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TOTW_LaunchSystemBPLibrary.generated.h"

class ATOTW_LaunchSystem;
class UAnimMontage;

/**
 * @class UTOTW_LaunchSystemBPLibrary
 * @brief Blueprint function library for easy character launching using motion warping
 * 
 * This library provides convenient Blueprint functions to launch characters to specific
 * points using the motion warping system.
 */
UCLASS()
class GMASEXSKG_API UTOTW_LaunchSystemBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Find or create a launch system in the world
	UFUNCTION(BlueprintCallable, Category = "TOTW|Launch System", meta = (WorldContext = "WorldContextObject"))
	static ATOTW_LaunchSystem* GetOrCreateLaunchSystem(const UObject* WorldContextObject);

	// Launch character to a specific location
	UFUNCTION(BlueprintCallable, Category = "TOTW|Launch System", meta = (WorldContext = "WorldContextObject"))
	static void LaunchCharacterToLocation(const UObject* WorldContextObject, AActor* Character, const FVector& TargetLocation, float LaunchHeight = 300.0f);

	// Launch character to another actor
	UFUNCTION(BlueprintCallable, Category = "TOTW|Launch System", meta = (WorldContext = "WorldContextObject"))
	static void LaunchCharacterToActor(const UObject* WorldContextObject, AActor* Character, AActor* TargetActor, float LaunchHeight = 300.0f);

	// Launch character to a component
	UFUNCTION(BlueprintCallable, Category = "TOTW|Launch System", meta = (WorldContext = "WorldContextObject"))
	static void LaunchCharacterToComponent(const UObject* WorldContextObject, AActor* Character, USceneComponent* TargetComponent, float LaunchHeight = 300.0f);

	// Launch character with custom montage
	UFUNCTION(BlueprintCallable, Category = "TOTW|Launch System", meta = (WorldContext = "WorldContextObject"))
	static void LaunchCharacterWithMontage(const UObject* WorldContextObject, AActor* Character, UAnimMontage* LaunchMontage, const FVector& TargetLocation, float LaunchHeight = 300.0f);

	// Check if character is currently launching
	UFUNCTION(BlueprintPure, Category = "TOTW|Launch System", meta = (WorldContext = "WorldContextObject"))
	static bool IsCharacterLaunching(const UObject* WorldContextObject, AActor* Character);

	// Cancel character launch
	UFUNCTION(BlueprintCallable, Category = "TOTW|Launch System", meta = (WorldContext = "WorldContextObject"))
	static void CancelCharacterLaunch(const UObject* WorldContextObject, AActor* Character);

	// Utility function to find launch system in world
	UFUNCTION(BlueprintPure, Category = "TOTW|Launch System", meta = (WorldContext = "WorldContextObject"))
	static ATOTW_LaunchSystem* FindLaunchSystem(const UObject* WorldContextObject);
}; 