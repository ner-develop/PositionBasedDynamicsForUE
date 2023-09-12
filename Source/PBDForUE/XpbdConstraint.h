// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XpbdConstraint.generated.h"

class UXpbdParticleData;

/**
 * 
 */
UCLASS()
class PBDFORUE_API UXpbdConstraint : public UObject
{
	GENERATED_BODY()

public:
	double λ;
	double α;
	UPROPERTY()
	TObjectPtr<UXpbdParticleData> P1Ref;
	UPROPERTY()
	TObjectPtr<UXpbdParticleData> P2Ref;
	double D;

	void Initialize(TObjectPtr<UXpbdParticleData> P1, TObjectPtr<UXpbdParticleData> P2);
	double ComputeΔλ(double Δt) const;
	FVector ComputeΔx(double Δλ) const;
};
