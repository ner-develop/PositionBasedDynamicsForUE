#include "XpbdSimulatorActorComponent.h"

#include "ParticleMeshComponent.h"
#include "XpbdConstraint.h"
#include "XpbdParticleData.h"

UXpbdSimulatorActorComponent::UXpbdSimulatorActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UXpbdSimulatorActorComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<UParticleMeshComponent*> ParticleMeshComponents;
	GetOwner()->GetComponents<UParticleMeshComponent>(ParticleMeshComponents);
	ParticleMeshComponents.Sort([](const UParticleMeshComponent& A, const UParticleMeshComponent& B) {
		return A.GetAlignmentOrder() < B.GetAlignmentOrder();
	});

	ParticleArray.Empty();
	for (int i = 0; i < ParticleMeshComponents.Num(); ++i)
	{
		TObjectPtr<UXpbdParticleData> ParticleData = NewObject<UXpbdParticleData>(this);
		ParticleArray.Add(ParticleData);
		ParticleData->ParticleMeshComponentRef = ParticleMeshComponents[i];

		ParticleData->bIsFixed = i == 0;
		ParticleData->InitialPosition = ParticleData->ParticleMeshComponentRef->GetComponentLocation();

		ParticleData->X = ParticleData->InitialPosition;
		ParticleData->M = ParticleData->ParticleMeshComponentRef->GetMass();
		ParticleData->W = 1.0 / ParticleData->M;
		ParticleData->V = FVector::ZeroVector;
	}

	ConstraintArray.Empty();
	for (int i = 0; i < ParticleArray.Num() - 1; ++i)
	{
		TObjectPtr<UXpbdConstraint> Constraint = NewObject<UXpbdConstraint>(this);
		ConstraintArray.Add(Constraint);
		Constraint->Initialize(ParticleArray[i], ParticleArray[i + 1]);
	}
}

void UXpbdSimulatorActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Step(DeltaTime);

	DrawDebugString(
		GetWorld(),
		GetOwner()->GetActorLocation(),
		TEXT("XPBD"),
		nullptr,
		FColor::White,
		0.f
	);
}

void UXpbdSimulatorActorComponent::Step(const double Δt)
{
	if (!bIsEnabledSimulation) { return; }

	// 固定点の位置をデータに反映
	for (const auto& P : ParticleArray)
	{
		if (!P->bIsFixed) { continue; }
		P->X = P->ParticleMeshComponentRef->GetComponentLocation();
	}

	Simulate(Δt);

	// データの位置を反映
	for (const auto& P : ParticleArray)
	{
		if (const TObjectPtr<UParticleMeshComponent> ParticleMeshComponent = P->ParticleMeshComponentRef.Get())
		{
			ParticleMeshComponent->SetWorldLocation(P->X);
		}
	}
}

void UXpbdSimulatorActorComponent::Simulate(const double Δt)
{
	for (const auto& P : ParticleArray)
	{
		// (1) predict position
		const FVector XTilda = P->X + Δt * P->V + Gravity * Δt * Δt * 0.5;

		// (3) initialize solve
		P->Xi = XTilda;
	}

	for (const auto& C : ConstraintArray)
	{
		// (4) initialize multipliers
		C->λ = 0;
		C->α = Flexibility;
	}

	// (5) ~ (13)
	int32 i = 0;
	while (i < Iterations)
	{
		for (const auto& C : ConstraintArray)
		{
			// compute
			const double Δλ = C->ComputeΔλ(Δt);
			const FVector Δx = C->ComputeΔx(Δλ);

			// update
			C->λ = C->λ + Δλ;
			C->P1Ref->Xi = C->P1Ref->Xi + C->P1Ref->W * Δx;
			C->P2Ref->Xi = C->P2Ref->Xi - C->P2Ref->W * Δx;
		}
		i = i + 1;
	}

	for (const auto& P : ParticleArray)
	{
		if (P->bIsFixed) { continue; }
		// (16) update velocity
		P->V = (P->Xi - P->X) / Δt;
		// (15) update position
		P->X = P->Xi;
	}
}

