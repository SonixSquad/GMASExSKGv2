// TOTW_FACTION_A_BASE DERIVED FROM TOTW_MWPAWN_BASE


#include "TOTW_Faction_A_Base.h"


ATOTW_Faction_A_Base::ATOTW_Faction_A_Base(const FObjectInitializer& ObjectInitializer)
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

