// Fill out your copyright notice in the Description page of Project Settings.


#include "XpbdConstraint.h"

#include "XpbdParticleData.h"

void UXpbdConstraint::Initialize(const TObjectPtr<UXpbdParticleData> P1, const TObjectPtr<UXpbdParticleData> P2)
{
	P1Ref = P1;
	P2Ref = P2;
	D = FVector::Distance(P1->X, P2->X);
}

double UXpbdConstraint::ComputeΔλ(const double Δt) const
{
	const FVector Vector = P2Ref->Xi - P1Ref->Xi;
	const double Distance = Vector.Size();
	const double C = Distance - D;
	const double αTilda = α / (Δt * Δt);
	const FVector GradC = Vector / Distance;
	const float InvM = P1Ref->W + P2Ref->W;
	return (C - αTilda * λ) / (GradC.Dot(InvM * GradC) + αTilda);
}

FVector UXpbdConstraint::ComputeΔx(const double Δλ) const
{
	const FVector GradC = (P2Ref->X - P1Ref->X).GetSafeNormal();
	return GradC * Δλ;
}
