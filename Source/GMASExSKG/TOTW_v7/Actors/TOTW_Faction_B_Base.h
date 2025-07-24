// TOTW_FACTION_B_BASE DERIVED FROM TOTW_MWPAWN_BASE

#pragma once

#include "CoreMinimal.h"
#include "TOTW_MWPawn_Base.h"
#include "TOTW_Faction_B_Base.generated.h"

UCLASS()
class GMASEXSKG_API ATOTW_Faction_B_Base : public ATOTW_MWPawn_Base
{
	GENERATED_BODY()

public:
	explicit ATOTW_Faction_B_Base(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintPure, Category = "Camera")
	bool IsFirstPerson() const { return bIsFirstPerson; }

protected:
	

	// Camera state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	bool bIsFirstPerson = false;

};
