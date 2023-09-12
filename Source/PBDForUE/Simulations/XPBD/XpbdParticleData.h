#pragma once

#include "CoreMinimal.h"
#include "XpbdParticleData.generated.h"

class UParticleMeshComponent;

/**
 * 
 */
UCLASS()
class PBDFORUE_API UXpbdParticleData : public UObject
{
	GENERATED_BODY()

public:
	bool bIsFixed;
	FVector X;
	FVector Xi;
	FVector V;
	double M;
	double W;
	FVector InitialPosition;

	UPROPERTY()
	TObjectPtr<UParticleMeshComponent> ParticleMeshComponentRef;
};
