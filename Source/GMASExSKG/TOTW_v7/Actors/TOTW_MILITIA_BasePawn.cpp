// TOTW MILITIA PAWN CLASS DERIVED FROM TOTW_MWPAWN_BASE
// SPECIFICALLY FOR MILITIA RELATED CALCULATION HEAVY FUNCTIONS + COMPONENTS

#include "TOTW_MILITIA_BasePawn.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GMASExSKG/TOTW_v7/Components/TOTW_CustomMovement.h"
#include "Utility/GMASUtilities.h"

ATOTW_MILITIA_BasePawn::ATOTW_MILITIA_BasePawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Start in FIRST person by default
	bIsFirstPerson = true;

	// Ensure camera is set up correctly
	if (TPCam)
	{
		TPCam->SetActive(false);
	}
}

