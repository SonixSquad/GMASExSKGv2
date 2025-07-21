// TOTW PAWN CLASS DERIVED FROM TOTW_GMCPAWN

#pragma once

#include "CoreMinimal.h"
#include "TOTW_GMCPawn_Base.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TOTW_GMCPawn.generated.h"

class ATOTW_CustomMovement;

/**
 * @class ATOTW_GMCPawn
 * @brief A pawn class that extends the base template pawn.
 *
 * This class serves as a third-person convenience pawn with additional components such as a skeletal mesh,
 * a spring arm, and a follow camera.
 */
UCLASS()
class GMASEXSKG_API ATOTW_GMCPawn : public ATOTW_GMCPawn_Base
{
	GENERATED_BODY()

public:
	explicit ATOTW_GMCPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure, Category = "Camera")
	bool IsFirstPerson() const { return bIsFirstPerson; }

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
/**
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", DisplayName="SK_Mesh", meta=(AllowPrivateAccess=true))
	TObjectPtr<USkeletalMeshComponent> SKMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", DisplayName="TP_Arm", meta=(AllowPrivateAccess=true))
	TObjectPtr<USpringArmComponent> TPArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", DisplayName="TP_Cam", meta=(AllowPrivateAccess=true))
	TObjectPtr<UCameraComponent> TPCam;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", DisplayName="FP_Cam", meta=(AllowPrivateAccess=true))
	TObjectPtr<UCameraComponent> FPCam;
	**/
	// Camera state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	bool bIsFirstPerson = false;

};