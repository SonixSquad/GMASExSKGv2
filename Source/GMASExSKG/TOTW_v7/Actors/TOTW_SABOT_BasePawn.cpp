// TOTW SABOT PAWN CLASS DERIVED FROM TOTW_MWPAWN_BASE
// SPECIFICALLY FOR SABOT RELATED CALCULATION HEAVY FUNCTIONS + COMPONENTS


#include "TOTW_SABOT_BasePawn.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"


ATOTW_SABOT_BasePawn::ATOTW_SABOT_BasePawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	USceneComponent *TemporaryCapsule = GetComponentByClass<UCapsuleComponent>();
	

	// Start in third person by default
	bIsFirstPerson = false;

	TPCam->SetActive(true);
}

