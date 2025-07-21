// TOTW PAWN CLASS DERIVED FROM TOTW_MWPAWN_BASE


#include "TOTW_MWPawn.h"
#include "Components/CapsuleComponent.h"


ATOTW_MWPawn::ATOTW_MWPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	USceneComponent *TemporaryCapsule = GetComponentByClass<UCapsuleComponent>();

	
}

