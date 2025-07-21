#include "TOTW_LaunchSystemBPLibrary.h"
#include "TOTW_LaunchSystem.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

ATOTW_LaunchSystem* UTOTW_LaunchSystemBPLibrary::GetOrCreateLaunchSystem(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
		return nullptr;

	// First try to find an existing launch system
	ATOTW_LaunchSystem* ExistingSystem = FindLaunchSystem(WorldContextObject);
	if (ExistingSystem)
		return ExistingSystem;

	// If none exists, create one
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	ATOTW_LaunchSystem* NewSystem = World->SpawnActor<ATOTW_LaunchSystem>(ATOTW_LaunchSystem::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	
	if (NewSystem)
	{
		UE_LOG(LogTemp, Log, TEXT("TOTW_LaunchSystemBPLibrary: Created new launch system"));
	}
	
	return NewSystem;
}

void UTOTW_LaunchSystemBPLibrary::LaunchCharacterToLocation(const UObject* WorldContextObject, AActor* Character, const FVector& TargetLocation, float LaunchHeight)
{
	ATOTW_LaunchSystem* LaunchSystem = GetOrCreateLaunchSystem(WorldContextObject);
	if (LaunchSystem)
	{
		LaunchSystem->LaunchCharacterToLocation(Character, TargetLocation, LaunchHeight);
	}
}

void UTOTW_LaunchSystemBPLibrary::LaunchCharacterToActor(const UObject* WorldContextObject, AActor* Character, AActor* TargetActor, float LaunchHeight)
{
	ATOTW_LaunchSystem* LaunchSystem = GetOrCreateLaunchSystem(WorldContextObject);
	if (LaunchSystem)
	{
		LaunchSystem->LaunchCharacterToActor(Character, TargetActor, LaunchHeight);
	}
}

void UTOTW_LaunchSystemBPLibrary::LaunchCharacterToComponent(const UObject* WorldContextObject, AActor* Character, USceneComponent* TargetComponent, float LaunchHeight)
{
	ATOTW_LaunchSystem* LaunchSystem = GetOrCreateLaunchSystem(WorldContextObject);
	if (LaunchSystem)
	{
		LaunchSystem->LaunchCharacterToComponent(Character, TargetComponent, LaunchHeight);
	}
}

void UTOTW_LaunchSystemBPLibrary::LaunchCharacterWithMontage(const UObject* WorldContextObject, AActor* Character, UAnimMontage* LaunchMontage, const FVector& TargetLocation, float LaunchHeight)
{
	ATOTW_LaunchSystem* LaunchSystem = GetOrCreateLaunchSystem(WorldContextObject);
	if (LaunchSystem)
	{
		LaunchSystem->LaunchCharacterWithMontage(Character, LaunchMontage, TargetLocation, LaunchHeight);
	}
}

bool UTOTW_LaunchSystemBPLibrary::IsCharacterLaunching(const UObject* WorldContextObject, AActor* Character)
{
	ATOTW_LaunchSystem* LaunchSystem = FindLaunchSystem(WorldContextObject);
	if (LaunchSystem)
	{
		return LaunchSystem->IsCharacterLaunching(Character);
	}
	return false;
}

void UTOTW_LaunchSystemBPLibrary::CancelCharacterLaunch(const UObject* WorldContextObject, AActor* Character)
{
	ATOTW_LaunchSystem* LaunchSystem = FindLaunchSystem(WorldContextObject);
	if (LaunchSystem)
	{
		LaunchSystem->CancelLaunch(Character);
	}
}

ATOTW_LaunchSystem* UTOTW_LaunchSystemBPLibrary::FindLaunchSystem(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
		return nullptr;

	// Find all launch systems in the world
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, ATOTW_LaunchSystem::StaticClass(), FoundActors);
	
	if (FoundActors.Num() > 0)
	{
		return Cast<ATOTW_LaunchSystem>(FoundActors[0]);
	}
	
	return nullptr;
} 