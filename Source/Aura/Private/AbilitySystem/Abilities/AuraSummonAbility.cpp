// Copyright AM Company


#include "AbilitySystem/Abilities/AuraSummonAbility.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions;

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
	const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector);

	TArray<FVector> SpawnLocations;
	bool IsTrySpawnClose = true;
	for (int32 i = 0; i < NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);

		float RandomDistance;
		if (IsTrySpawnClose) RandomDistance = FMath::RandRange(MinSpawnDistance, MaxSpawnDistance / 2);
		else RandomDistance = FMath::RandRange(MaxSpawnDistance / 2, MaxSpawnDistance);
		const FVector ChosenSpawnLocation = Location + Direction * RandomDistance;

		SpawnLocations.Add(ChosenSpawnLocation);
		IsTrySpawnClose = !IsTrySpawnClose;
	}

	return SpawnLocations;
}
