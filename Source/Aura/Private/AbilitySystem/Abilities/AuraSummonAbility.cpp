//


#include "AbilitySystem/Abilities/AuraSummonAbility.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

TArray<FTransform> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions;

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
	TArray<FTransform> SpawnLocations;

	for(int32 i=0; i<NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);
		FRotator ChosenRotation = FRotator(0.f);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0.f, 0.f, 400.f), ChosenSpawnLocation - FVector(0.f, 0.f, 400.f), ECC_Visibility);

		if(Hit.bBlockingHit)
		{
			ChosenSpawnLocation = Hit.ImpactPoint;
			ChosenRotation = UKismetMathLibrary::MakeRotFromZX(Hit.ImpactNormal, Hit.ImpactNormal);
		}

		FTransform ChosenTransform(ChosenRotation, ChosenSpawnLocation, FVector(1.f));
		SpawnLocations.Add(ChosenTransform);
	}

	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	if(MinionClasses.Num()<1) return nullptr;
	const int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[Selection];
}
