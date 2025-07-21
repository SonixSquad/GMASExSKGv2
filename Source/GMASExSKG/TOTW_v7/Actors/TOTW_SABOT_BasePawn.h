// TOTW SABOT PAWN CLASS DERIVED FROM TOTW_MWPAWN_BASE
// SPECIFICALLY FOR SABOT RELATED CALCULATION HEAVY FUNCTIONS + COMPONENTS

#pragma once

#include "CoreMinimal.h"

#include "GMCE_MotionWarpingComponent.h"
#include "GMCE_MotionWarpSubject.h"
#include "TOTW_MWPawn_Base.h"
#include "Actors/GMAS_Pawn.h"
#include "UObject/Object.h"
#include "TOTW_SABOT_BasePawn.generated.h"

UCLASS()
class GMASEXSKG_API ATOTW_SABOT_BasePawn : public ATOTW_MWPawn_Base
{
	GENERATED_BODY()

public:
	explicit ATOTW_SABOT_BasePawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintPure, Category = "Camera")
	bool IsFirstPerson() const { return bIsFirstPerson; }

protected:
	

	// Camera state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	bool bIsFirstPerson = false;

};