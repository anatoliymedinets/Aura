// Copyright AM Company


#include "AbilitySystem/Abilities/AuraSummonAbility.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions;

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);

	// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + LeftOfSpread * MaxSpawnDistance, 4.f, FLinearColor::Green, 2.f, 2.f);
	// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector) * MaxSpawnDistance, 4.f, FLinearColor::Green, 2.f, 2.f);

	TArray<FVector> SpawnLocations;
	bool IsTrySpawnClose = true; // need only if IsLadderSpawn = true

	for (int32 i = 0; i < NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);

		float RandomDistance;
		if (IsLadderSpawn)
		{
			if (IsTrySpawnClose) RandomDistance = FMath::RandRange(MinSpawnDistance, MaxSpawnDistance / 2);
			else RandomDistance = FMath::RandRange(MaxSpawnDistance / 2, MaxSpawnDistance);
		}
		else 
		{
			RandomDistance = FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);
		}
		
		FVector ChosenSpawnLocation = Location + Direction * RandomDistance;

		// Проверка на неровности (холм, ступеньки ...)
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0, 0, 400.f), ChosenSpawnLocation + FVector(0, 0, -400.f),ECC_Visibility);

		if (Hit.bBlockingHit)
		{
			ChosenSpawnLocation = Hit.ImpactPoint;
		}


		SpawnLocations.Add(ChosenSpawnLocation);
		IsTrySpawnClose = !IsTrySpawnClose;
	}

	return SpawnLocations;
}
