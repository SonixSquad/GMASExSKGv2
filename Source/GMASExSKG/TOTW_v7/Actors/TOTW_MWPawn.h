// TOTW PAWN CLASS DERIVED FROM TOTW_MWPAWN_BASE

#pragma once

#include "CoreMinimal.h"

#include "TOTW_MWPawn_Base.h"
#include "GMCE_MotionWarpingComponent.h"
#include "GMCE_MotionWarpSubject.h"
#include "Actors/GMAS_Pawn.h"
#include "UObject/Object.h"
#include "TOTW_MWPawn.generated.h"

class ATOTW_CustomMovement;

/**
 * @class ATOTW_MWPawn
 * @brief A pawn class that extends the base template pawn.
 *
 * This class serves as a third-person convenience pawn with additional components such as a skeletal mesh,
 * a spring arm, and a follow camera.
 */
UCLASS()
class GMASEXSKG_API ATOTW_MWPawn : public ATOTW_MWPawn_Base
{
	GENERATED_BODY()

public:
	explicit ATOTW_MWPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure, Category = "Camera")
	bool IsFirstPerson() const { return bIsFirstPerson; }

protected:

	

	// Camera state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	bool bIsFirstPerson = false;

};

