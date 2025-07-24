// TOTW_FACTION_B_BASE DERIVED FROM TOTW_MWPAWN_BASE


#include "TOTW_Faction_B_Base.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"


ATOTW_Faction_B_Base::ATOTW_Faction_B_Base(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	USceneComponent *TemporaryCapsule = GetComponentByClass<UCapsuleComponent>();
	

	// Start in third person by default
	bIsFirstPerson = false;

	TPCam->SetActive(true);
}

