// TOTW BASE PAWN CLASS DERIVED FROM GMCE_MOTIONWARPPAWN

#pragma once

#include "CoreMinimal.h"
#include "GMASExSKGMovementComponent.h"
#include "GMCAbilityComponent.h"
#include "GMCE_MotionWarpSubject.h"
#include "GMCE_OrganicMovementCmp.h"
#include "GMCE_MotionWarpPawn.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TOTW_MWPawn_Base.generated.h"

UCLASS()
class GMASEXSKG_API ATOTW_MWPawn_Base : public AGMCE_MotionWarpPawn
{
	GENERATED_BODY()
	
public:

	ATOTW_MWPawn_Base(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "GMCExTemplatePawn")
	virtual void Respawn();

	void Respawn_Internal();
	
	UFUNCTION(Client, Reliable)
	void CL_Respawn();

	UFUNCTION(Server, Reliable)
	void SV_Respawn();
	
	UFUNCTION()
	void OnRespawnCallback();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Respawn")
	void OnRespawn();
	
	// GMCExtendedAnimation motion warping interface
	virtual USkeletalMeshComponent* MotionWarping_GetMeshComponent() const override;
	virtual float MotionWarping_GetCollisionHalfHeight() const override;
	virtual FQuat MotionWarping_GetRotationOffset() const override;
	virtual FVector MotionWarping_GetTranslationOffset() const override;
	virtual FAnimMontageInstance* GetRootMotionAnimMontageInstance(USkeletalMeshComponent* MeshComponent) const override;
	virtual UGMCE_OrganicMovementCmp* GetGMCExMovementComponent() const override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", DisplayName="SK_Mesh", meta=(AllowPrivateAccess=true))
	TObjectPtr<USkeletalMeshComponent> SKMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", DisplayName="TP_Arm", meta=(AllowPrivateAccess=true))
	TObjectPtr<USpringArmComponent> TPArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", DisplayName="TP_Cam", meta=(AllowPrivateAccess=true))
	TObjectPtr<UCameraComponent> TPCam;
	
	// A convenience setting; if this is set, the given input mapping context will be added with
	// priority 0 automatically.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", DisplayName="Capsule", meta=(AllowPrivateAccess=true))
	TObjectPtr<UCapsuleComponent> CapsuleComponent;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<UGMASExSKGMovementComponent> MovementComponent;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<UGMC_AbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<UGMCE_MotionWarpingComponent> MotionWarpingComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<USkeletalMeshComponent> MotionWarpingMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Respawn")
	FGameplayTagContainer RespawnRemovesEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Respawn")
	FGameplayTagContainer RespawnRemovesActiveTags;
};

