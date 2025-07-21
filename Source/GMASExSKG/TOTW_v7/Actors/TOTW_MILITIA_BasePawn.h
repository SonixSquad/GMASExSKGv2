// TOTW MILITIA PAWN CLASS DERIVED FROM TOTW_MWPAWN_BASE
// SPECIFICALLY FOR MILITIA RELATED CALCULATION HEAVY FUNCTIONS + COMPONENTS

#pragma once

#include "CoreMinimal.h"
#include "GMCE_MotionWarpingComponent.h"
#include "GMCE_MotionWarpSubject.h"
#include "TOTW_MWPawn_Base.h"
#include "Actors/GMAS_Pawn.h"
#include "UObject/Object.h"
#include "TOTW_MILITIA_BasePawn.generated.h"

UCLASS()
class GMASEXSKG_API ATOTW_MILITIA_BasePawn : public ATOTW_MWPawn_Base
{
	GENERATED_BODY()

public:
	explicit ATOTW_MILITIA_BasePawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintPure, Category = "Camera")
	bool IsFirstPerson() const { return bIsFirstPerson; }

protected:

	

	// Camera state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	bool bIsFirstPerson = true;

};