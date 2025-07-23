#pragma once

#include "CoreMinimal.h"
#include "Ability/GMCAbility.h"
#include "TOTW_v7/Systems/TOTW_LaunchSystem.h"
#include "GA_Dash.generated.h"

class UAnimMontage;
class ATOTW_LaunchSystem;

/**
 * @class UGA_Dash
 * @brief A dash ability that launches the character forward using motion warping
 * 
 * This ability uses the TOTW_LaunchSystem to create a smooth dash movement
 * with animation and motion warping for realistic character movement.
 */
UCLASS(BlueprintType, Blueprintable)
class GMASEXSKG_API UGA_Dash : public UGMCAbility
{
	GENERATED_BODY()
	
public:
	UGA_Dash();

protected:
	virtual void BeginAbility() override;
	virtual void EndAbility() override;

public:
	// Dash Configuration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Settings")
	float DashDistance = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Settings")
	float DashHeight = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Settings")
	float DashSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Settings")
	UAnimMontage* DashMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Settings")
	bool bUseArcTrajectory = false;

	// Cooldown and Cost
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Settings")
	float DashCooldown = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Settings")
	float StaminaCost = 25.0f;

protected:
	// Internal Functions
	void PerformDash();
	FVector CalculateDashTarget() const;

	// Launch System Reference
	UPROPERTY()
	ATOTW_LaunchSystem* LaunchSystem;

	// State
	UPROPERTY()
	bool bIsDashing = false;

	// Events
	UFUNCTION(BlueprintImplementableEvent, Category = "Dash Ability")
	void OnDashStarted();

	UFUNCTION(BlueprintImplementableEvent, Category = "Dash Ability")
	void OnDashFinished();

	UFUNCTION(BlueprintImplementableEvent, Category = "Dash Ability")
	void OnDashInterrupted();
}; 