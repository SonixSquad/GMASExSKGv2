// TOTW BASE PAWN CLASS DERIVED FROM GMCE_MOTIONWARPPAWN
// SPECIFICALLY FOR SHARED+COMMON CALCULATION HEAVY FUNCTIONS + COMPONENTS ACROSS ALL FACTIONS

#include "TOTW_MWPawn_Base.h"

#include "Camera/CameraComponent.h"
#include "TOTW_v7/Data/CharSetup_DataAsset.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GMASExSKGMovementComponent.h"
#include "Utility/GMASUtilities.h"


ATOTW_MWPawn_Base::ATOTW_MWPawn_Base(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	// Create root component first
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(CapsuleComponent);
	CapsuleComponent->SetCapsuleRadius(30.f);
	CapsuleComponent->SetCapsuleHalfHeight(90.f);
	CapsuleComponent->bEditableWhenInherited = true;
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComponent->SetCollisionObjectType(ECC_Pawn);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	// Create skeletal mesh component
	SKMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Mesh"));
	SKMesh->bEditableWhenInherited = true;
	SKMesh->SetupAttachment(CapsuleComponent);
	SKMesh->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	SKMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	SKMesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	// Create spring arm component
	TPArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TP_Arm"));
	TPArm->bEditableWhenInherited = true;
	TPArm->SetupAttachment(CapsuleComponent);
	TPArm->TargetArmLength = 400.f;
	TPArm->bUsePawnControlRotation = true;

	// Create camera component
	TPCam = CreateDefaultSubobject<UCameraComponent>(TEXT("TP_Cam"));
	TPCam->bEditableWhenInherited = true;
	TPCam->SetupAttachment(TPArm);
	TPCam->bUsePawnControlRotation = false;
	
	// Create ability system component
	AbilitySystemComponent = CreateDefaultSubobject<UGMC_AbilitySystemComponent>(TEXT("Ability Component"));
	
	// Create motion warping component
	MotionWarpingComponent = ObjectInitializer.CreateDefaultSubobject<UGMCE_MotionWarpingComponent>(this, TEXT("MotionWarpingComponent"));
	
	OverrideInputComponentClass = UEnhancedInputComponent::StaticClass();

#if WITH_EDITOR
	UGMASUtilities::SetPropertyFlagsSafe(StaticClass(), TEXT("SKMesh"), CPF_DisableEditOnInstance);
	UGMASUtilities::SetPropertyFlagsSafe(StaticClass(), TEXT("TPArm"), CPF_DisableEditOnInstance);
	UGMASUtilities::SetPropertyFlagsSafe(StaticClass(), TEXT("TPCam"), CPF_DisableEditOnInstance);
	UGMASUtilities::SetPropertyFlagsSafe(StaticClass(), TEXT("CapsuleComponent"), CPF_DisableEditOnInstance);
	UGMASUtilities::SetPropertyFlagsSafe(StaticClass(), TEXT("AbilitySystemComponent"), CPF_DisableEditOnInstance);
	UGMASUtilities::SetPropertyFlagsSafe(StaticClass(), TEXT("MotionWarpingComponent"), CPF_DisableEditOnInstance);
#endif
	
}

// Called when the game starts or when spawned
void ATOTW_MWPawn_Base::BeginPlay()
{
	Super::BeginPlay();
	
	// Debug spawn location
	UE_LOG(LogActor, Warning, TEXT("%s spawned at location: %s"), *GetName(), *GetActorLocation().ToString());
	
	// Check if we're at a valid spawn location
	FVector CurrentLocation = GetActorLocation();
	if (CurrentLocation.Z < -1000.0f || FMath::IsNaN(CurrentLocation.X) || FMath::IsNaN(CurrentLocation.Y) || FMath::IsNaN(CurrentLocation.Z))
	{
		UE_LOG(LogActor, Error, TEXT("%s spawned at invalid location: %s"), *GetName(), *CurrentLocation.ToString());
	}
	
	MovementComponent = GetComponentByClass<UGMASExSKGMovementComponent>();

	if (IsValid(MovementComponent))
	{
		MovementComponent->OnRespawnDelegate.BindUFunction(this, "OnRespawnCallback");
	}
	else
	{
		UE_LOG(LogActor, Error, TEXT("%s has no valid movement component!"), *GetName());
	}
	

	if (MotionWarpingMeshComponent == nullptr)
	{
		MotionWarpingMeshComponent = GetComponentByClass<USkeletalMeshComponent>();
	}

	if (IsValid(MovementComponent) && IsValid(MotionWarpingComponent))
	{
		MotionWarpingComponent->BindToMovementComponent();
	}
	
	// Ensure we're not being affected by any motion warping on spawn
	if (IsValid(MotionWarpingComponent))
	{
		MotionWarpingComponent->RemoveAllWarpTargets();
		UE_LOG(LogActor, Warning, TEXT("%s motion warping component cleared"), *GetName());
	}
	
	// Debug component hierarchy
	UE_LOG(LogActor, Warning, TEXT("%s root component: %s"), *GetName(), 
		RootComponent ? *RootComponent->GetName() : TEXT("NULL"));
	UE_LOG(LogActor, Warning, TEXT("%s capsule component: %s"), *GetName(), 
		CapsuleComponent ? *CapsuleComponent->GetName() : TEXT("NULL"));
	UE_LOG(LogActor, Warning, TEXT("%s skeletal mesh component: %s"), *GetName(), 
		SKMesh ? *SKMesh->GetName() : TEXT("NULL"));
}

// Called to bind functionality to input
void ATOTW_MWPawn_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (const UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent); InputMappingContext && EnhancedInput)
	{
		if (const AGMC_PlayerController* Player = Cast<AGMC_PlayerController>(GetController()))
		{
			if (Player->GetLocalPlayer() != nullptr)
			{
				if (UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Player->GetLocalPlayer()))
				{
					// Add our default system as the lowest mapping context.
					InputSystem->AddMappingContext(InputMappingContext, 0);
				}				
			}
		}
	}
	
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATOTW_MWPawn_Base::Respawn()
{
	// Respawn is not relevant for simulated proxies.
	if (GetLocalRole() == ROLE_SimulatedProxy) return;

	Respawn_Internal();

	// If we're standalone, this is enough.
	if (GetNetMode() == NM_Standalone) return;

	// If we're a client, call the server version of respawn.
	if (GetNetMode() == NM_Client)
	{
		SV_Respawn();
		return;
	}

	// If we're here, we're a server; call the client version.
	CL_Respawn();
}

void ATOTW_MWPawn_Base::Respawn_Internal()
{
	if (!MovementComponent) return;

	MovementComponent->Respawn();	
}

void ATOTW_MWPawn_Base::CL_Respawn_Implementation()
{
	// Any other respawn logic is not relevant for simulated proxies.
	if (GetLocalRole() == ROLE_SimulatedProxy) return;
	
	Respawn_Internal();
}

void ATOTW_MWPawn_Base::SV_Respawn_Implementation()
{
	Respawn_Internal();
}

void ATOTW_MWPawn_Base::OnRespawnCallback()
{
	if (!AbilitySystemComponent) return;

	for (const FGameplayTag& ActiveTag : RespawnRemovesActiveTags)
	{
		AbilitySystemComponent->RemoveActiveTag(ActiveTag);
	}

	for (const FGameplayTag& EffectTag : RespawnRemovesEffects)
	{
		AbilitySystemComponent->RemoveEffectByTag(EffectTag, -1, false);
	}

	OnRespawn();
}

void ATOTW_MWPawn_Base::OnRespawn_Implementation()
{
}

USkeletalMeshComponent* ATOTW_MWPawn_Base::MotionWarping_GetMeshComponent() const
{
	return MotionWarpingMeshComponent;
}

float ATOTW_MWPawn_Base::MotionWarping_GetCollisionHalfHeight() const
{
	if (!MovementComponent) return 0.f;
	
	return MovementComponent->GetRootCollisionHalfHeight(true);	
}

FQuat ATOTW_MWPawn_Base::MotionWarping_GetRotationOffset() const
{
	if (!MotionWarpingMeshComponent) return FQuat::Identity;
	
	return MotionWarpingMeshComponent->GetRelativeRotation().Quaternion();	
}

FVector ATOTW_MWPawn_Base::MotionWarping_GetTranslationOffset() const
{
	if (!MotionWarpingMeshComponent) return FVector::ZeroVector;

	return MotionWarpingMeshComponent->GetRelativeLocation();
}

FAnimMontageInstance* ATOTW_MWPawn_Base::GetRootMotionAnimMontageInstance(
	USkeletalMeshComponent* InMeshComponent) const
{
	// Explicitly specify which path to use by using virtual base class
	return AGMCE_MotionWarpPawn::GetRootMotionAnimMontageInstance(InMeshComponent);	
}

UGMCE_OrganicMovementCmp* ATOTW_MWPawn_Base::GetGMCExMovementComponent() const
{
	return MovementComponent;
}

void ATOTW_MWPawn_Base::OnRep_CharacterMesh()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_CharacterMesh called, ReplicatedCharacterMesh valid: %s, SKMesh valid: %s"), 
		ReplicatedCharacterMesh.IsValid() ? TEXT("True") : TEXT("False"),
		SKMesh ? TEXT("True") : TEXT("False"));
	
	if (ReplicatedCharacterMesh.IsValid())
	{
		USkeletalMesh* LoadedMesh = ReplicatedCharacterMesh.LoadSynchronous();
		if (LoadedMesh && SKMesh)
		{
			SKMesh->SetSkeletalMesh(LoadedMesh);
			UE_LOG(LogTemp, Log, TEXT("Replicated mesh applied: %s"), *LoadedMesh->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load mesh (%s) or SKMesh is null"), 
				LoadedMesh ? TEXT("Mesh loaded") : TEXT("Mesh failed to load"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ReplicatedCharacterMesh is not valid"));
	}
}

void ATOTW_MWPawn_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ATOTW_MWPawn_Base, ReplicatedCharacterMesh);
}

void ATOTW_MWPawn_Base::SetMesh(USkeletalMesh* NewMesh)
{
	UE_LOG(LogTemp, Warning, TEXT("SetMesh called with mesh: %s"), NewMesh ? *NewMesh->GetName() : TEXT("NULL"));
	
	if (NewMesh && SKMesh)
	{
		SKMesh->SetSkeletalMesh(NewMesh);
		UE_LOG(LogTemp, Log, TEXT("Mesh applied: %s"), *NewMesh->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to set mesh - NewMesh: %s, SKMesh: %s"), 
			NewMesh ? TEXT("Valid") : TEXT("NULL"),
			SKMesh ? TEXT("Valid") : TEXT("NULL"));
	}
}

void ATOTW_MWPawn_Base::SetMeshFromDataAsset(UCharSetup_DataAsset* DataAsset)
{
	// Only allow mesh setting on server or in standalone
	if (!HasAuthority() && GetNetMode() != NM_Standalone)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetMeshFromDataAsset called without authority - ignoring"));
		return;
	}

	if (DataAsset)
	{
		DataAsset->ApplySkeletalMesh(this);
		UE_LOG(LogTemp, Log, TEXT("Applied mesh from data asset: %s"), *DataAsset->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Data asset is null"));
	}
}

