// Fill out your copyright notice in the Description page of Project Settings.


#include "TOTW_GMCPawn_Base.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GMASExSKGMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Utility/GMASUtilities.h"


ATOTW_GMCPawn_Base::ATOTW_GMCPawn_Base(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	// Add our basic goodies.
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
	
	AbilitySystemComponent = CreateDefaultSubobject<UGMC_AbilitySystemComponent>(TEXT("Ability Component"));
	MotionWarpingComponent = ObjectInitializer.CreateDefaultSubobject<UGMCE_MotionWarpingComponent>(this, TEXT("MotionWarpingComponent"));
	OverrideInputComponentClass = UEnhancedInputComponent::StaticClass();

#if WITH_EDITOR
	UGMASUtilities::SetPropertyFlagsSafe(StaticClass(), TEXT("CapsuleComponent"), CPF_DisableEditOnInstance);
	UGMASUtilities::SetPropertyFlagsSafe(StaticClass(), TEXT("AbilitySystemComponent"), CPF_DisableEditOnInstance);
	UGMASUtilities::SetPropertyFlagsSafe(StaticClass(), TEXT("MotionWarpingComponent"), CPF_DisableEditOnInstance);
#endif	
}

// Called when the game starts or when spawned
void ATOTW_GMCPawn_Base::BeginPlay()
{
	Super::BeginPlay();
	
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
}

// Called to bind functionality to input
void ATOTW_GMCPawn_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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

void ATOTW_GMCPawn_Base::Respawn()
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

void ATOTW_GMCPawn_Base::Respawn_Internal()
{
	if (!MovementComponent) return;

	MovementComponent->Respawn();	
}

void ATOTW_GMCPawn_Base::CL_Respawn_Implementation()
{
	// Any other respawn logic is not relevant for simulated proxies.
	if (GetLocalRole() == ROLE_SimulatedProxy) return;
	
	Respawn_Internal();
}

void ATOTW_GMCPawn_Base::SV_Respawn_Implementation()
{
	Respawn_Internal();
}

void ATOTW_GMCPawn_Base::OnRespawnCallback()
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

void ATOTW_GMCPawn_Base::OnRespawn_Implementation()
{
}

USkeletalMeshComponent* ATOTW_GMCPawn_Base::MotionWarping_GetMeshComponent() const
{
	return MotionWarpingMeshComponent;
}

float ATOTW_GMCPawn_Base::MotionWarping_GetCollisionHalfHeight() const
{
	if (!MovementComponent) return 0.f;
	
	return MovementComponent->GetRootCollisionHalfHeight(true);	
}

FQuat ATOTW_GMCPawn_Base::MotionWarping_GetRotationOffset() const
{
	if (!MotionWarpingMeshComponent) return FQuat::Identity;
	
	return MotionWarpingMeshComponent->GetRelativeRotation().Quaternion();	
}

FVector ATOTW_GMCPawn_Base::MotionWarping_GetTranslationOffset() const
{
	if (!MotionWarpingMeshComponent) return FVector::ZeroVector;

	return MotionWarpingMeshComponent->GetRelativeLocation();
}

FAnimMontageInstance* ATOTW_GMCPawn_Base::GetRootMotionAnimMontageInstance(
	USkeletalMeshComponent* InMeshComponent) const
{
	return IGMCE_MotionWarpSubject::GetRootMotionAnimMontageInstance(InMeshComponent);
}

UGMCE_OrganicMovementCmp* ATOTW_GMCPawn_Base::GetGMCExMovementComponent() const
{
	return MovementComponent;
}

