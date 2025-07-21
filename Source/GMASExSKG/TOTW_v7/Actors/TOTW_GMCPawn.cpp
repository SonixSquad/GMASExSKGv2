// TOTW PAWN CLASS DERIVED FROM TOTW_GMCPAWN


#include "TOTW_GMCPawn.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Utility/GMASUtilities.h"

ATOTW_GMCPawn::ATOTW_GMCPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	USceneComponent *TemporaryCapsule = GetComponentByClass<UCapsuleComponent>();
/**
	SKMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Mesh"));
	SKMesh->bEditableWhenInherited = true;
	SKMesh->SetupAttachment(TemporaryCapsule);
	SKMesh->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	SKMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	SKMesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	TPArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TP_Arm"));
	TPArm->bEditableWhenInherited = true;
	TPArm->SetupAttachment(TemporaryCapsule);
	TPArm->TargetArmLength = 400.f;
	TPArm->bUsePawnControlRotation = true;

	TPCam = CreateDefaultSubobject<UCameraComponent>(TEXT("TP_Cam"));
	TPCam->bEditableWhenInherited = true;
	TPCam->SetupAttachment(TPArm);
	TPCam->bUsePawnControlRotation = false;

	FPCam = CreateDefaultSubobject<UCameraComponent>(TEXT("FP_Cam"));
	FPCam->bEditableWhenInherited = true;
	FPCam->SetupAttachment(SKMesh, TEXT("S_Camera")); // Attach to S_Camera socket
	FPCam->bUsePawnControlRotation = true;

#if WITH_EDITOR
	UGMASUtilities::SetPropertyFlagsSafe(StaticClass(), TEXT("SKMesh"), CPF_DisableEditOnInstance);
	UGMASUtilities::SetPropertyFlagsSafe(StaticClass(), TEXT("TPArm"), CPF_DisableEditOnInstance);
	UGMASUtilities::SetPropertyFlagsSafe(StaticClass(), TEXT("TPCam"), CPF_DisableEditOnInstance);
//	UGMASUtilities::SetPropertyFlagsSafe(StaticClass(), TEXT("FPCam"), CPF_DisableEditOnInstance);
#endif	

	// Start in third person by default
	bIsFirstPerson = false;
//	FPCam->SetActive(false);
	TPCam->SetActive(true);
	**/
}

void ATOTW_GMCPawn::BeginPlay()
{
	Super::BeginPlay();
	
}


