#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GMCE_MotionWarpingComponent.h"
#include "GMCE_MotionWarpTarget.h"
#include "TOTW_LaunchSystem.generated.h"

class UAnimMontage;
class UGMCE_OrganicMovementCmp;

/**
 * @class ATOTW_LaunchSystem
 * @brief A system for launching characters to specific points using motion warping
 * 
 * This actor provides functionality to launch characters to designated points on the map
 * using GMCExtended's motion warping system. It can be used for abilities, environmental
 * interactions, or cinematic moments.
 */
UCLASS(BlueprintType, Blueprintable)
class GMASEXSKG_API ATOTW_LaunchSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	ATOTW_LaunchSystem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Launch Functions
	UFUNCTION(BlueprintCallable, Category = "Launch System")
	void LaunchCharacterToLocation(AActor* Character, const FVector& TargetLocation, float LaunchHeight = 300.0f);
	
	UFUNCTION(BlueprintCallable, Category = "Launch System")
	void LaunchCharacterToActor(AActor* Character, AActor* TargetActor, float LaunchHeight = 300.0f);
	
	UFUNCTION(BlueprintCallable, Category = "Launch System")
	void LaunchCharacterToComponent(AActor* Character, USceneComponent* TargetComponent, float LaunchHeight = 300.0f);

	// Launch with custom montage
	UFUNCTION(BlueprintCallable, Category = "Launch System")
	void LaunchCharacterWithMontage(AActor* Character, UAnimMontage* LaunchMontage, const FVector& TargetLocation, float LaunchHeight = 300.0f);

	// Utility Functions
	UFUNCTION(BlueprintPure, Category = "Launch System")
	bool IsCharacterLaunching(AActor* Character) const;
	
	UFUNCTION(BlueprintCallable, Category = "Launch System")
	void CancelLaunch(AActor* Character);

protected:
	// Launch Configuration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launch Settings")
	UAnimMontage* DefaultLaunchMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launch Settings")
	float DefaultLaunchHeight = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launch Settings")
	float LaunchSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launch Settings")
	bool bUseArcTrajectory = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launch Settings")
	float ArcHeight = 200.0f;

	// Motion Warping Target Name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Warping")
	FName LaunchTargetName = TEXT("LaunchTarget");

	// Currently launching characters
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Launch System")
	TArray<AActor*> LaunchingCharacters;

	// Internal Functions
	void SetupMotionWarping(AActor* Character, const FVector& TargetLocation, float Height);
	void CreateLaunchTrajectory(const FVector& StartLocation, const FVector& TargetLocation, float Height, TArray<FVector>& OutTrajectory);
	FVector CalculateLaunchVelocity(const FVector& StartLocation, const FVector& TargetLocation, float Height);
	
	// Helper to get motion warping component
	UGMCE_MotionWarpingComponent* GetMotionWarpingComponent(AActor* Character);
	UGMCE_OrganicMovementCmp* GetMovementComponent(AActor* Character);

	// Events
	UFUNCTION(BlueprintImplementableEvent, Category = "Launch System")
	void OnLaunchStarted(AActor* Character, const FVector& TargetLocation);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Launch System")
	void OnLaunchCompleted(AActor* Character, const FVector& TargetLocation);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Launch System")
	void OnLaunchCancelled(AActor* Character);
}; 