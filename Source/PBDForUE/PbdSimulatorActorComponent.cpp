// Fill out your copyright notice in the Description page of Project Settings.


#include "PbdSimulatorActorComponent.h"
#include "ParticleMeshComponent.h"
#include "PbdVertex.h"

UPbdSimulatorActorComponent::UPbdSimulatorActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UPbdSimulatorActorComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<UParticleMeshComponent*> ParticleMeshComponents;
	GetOwner()->GetComponents<UParticleMeshComponent>(ParticleMeshComponents);
	ParticleMeshComponents.Sort([](const UParticleMeshComponent& A, const UParticleMeshComponent& B) {
		return A.GetAlignmentOrder() < B.GetAlignmentOrder();
	});

	// (1) ~ (3)
	VertexArray.Empty();
	for (int i = 0; i < ParticleMeshComponents.Num(); ++i)
	{
		TObjectPtr<UPbdVertexData> Vertex = NewObject<UPbdVertexData>(this);
		VertexArray.Add(Vertex);
		Vertex->ParticleMeshComponentRef = ParticleMeshComponents[i];

		Vertex->bIsFixed = i == 0;
		Vertex->InitialPosition = Vertex->ParticleMeshComponentRef->GetComponentLocation();
		Vertex->X = Vertex->InitialPosition;
		Vertex->V = FVector::ZeroVector;
		Vertex->W = 1.f / Vertex->ParticleMeshComponentRef->GetMass();
	}
}


void UPbdSimulatorActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Step(DeltaTime);
}

void UPbdSimulatorActorComponent::Step(float DeltaTime)
{
	// 固定点の位置をデータに反映
	for (const auto& Vertex: VertexArray)
	{
		if (!Vertex->bIsFixed) { continue; }
		if (const TObjectPtr<UParticleMeshComponent> ParticleMeshComponent = Vertex->ParticleMeshComponentRef.Get())
		{
			Vertex->X = ParticleMeshComponent->GetComponentLocation();
		}
	}

	Simulate(DeltaTime);

	for (const auto& Vertex : VertexArray)
	{
		if (const TObjectPtr<UParticleMeshComponent> ParticleMeshComponent = Vertex->ParticleMeshComponentRef.Get())
		{
			ParticleMeshComponent->SetWorldLocation(Vertex->X);
		}
	}
}

void UPbdSimulatorActorComponent::Simulate(const float DeltaTime)
{
	// (5)
	for (const auto& Vertex : VertexArray)
	{
		if (Vertex->bIsFixed) { continue; }
		Vertex->V = Vertex->V + DeltaTime * Gravity;
	}

	// (6) スキップ
	// Damp Velocities

	// (7)
	for (const auto& Vertex : VertexArray)
	{
		Vertex->P = Vertex->X + DeltaTime * Vertex->V;
	}

	// (8) スキップ
	// GenerateCollisionConstraints

	// (9) ~ (11)
	const float K = 1 - FMath::Pow(1 - FMath::Clamp(Stiffness, 0.f, 1.f), 1.f / Iterations);
	for (int i = 0; i < Iterations; ++i)
	{
		ProjectConstraints(K);
	}

	// (12) ~ (15)
	for (const auto& Vertex : VertexArray)
	{
		if (Vertex->bIsFixed) { continue; }
		Vertex->V = (Vertex->P - Vertex->X) / DeltaTime;
		Vertex->X = Vertex->P;
	}

	// (16) スキップ
	// VelocityUpdate
}

void UPbdSimulatorActorComponent::ProjectConstraints(const float K)
{
	for (int i = 0; i < VertexArray.Num() - 1; ++i)
	{
		const TObjectPtr<UPbdVertexData> Vertex1 = VertexArray[i];
		const TObjectPtr<UPbdVertexData> Vertex2 = VertexArray[i + 1];

		const FVector P1P2 = Vertex1->P - Vertex2->P;
		const float P1P2Length = P1P2.Size();
		const float W1 = Vertex1->W;
		const float W2 = Vertex2->W;

		const FVector GradC = P1P2 / P1P2Length;
		const float C = Constraint(*Vertex1, *Vertex2);
		const float S = C / ((W1 + W2) * GradC.Dot(GradC));

		const FVector ΔP1 = -S * W1 * GradC;
		const FVector ΔP2 = +S * W2 * GradC;

		Vertex1->P += ΔP1 * K;
		Vertex2->P += ΔP2 * K;
	}
}

float UPbdSimulatorActorComponent::Constraint(const UPbdVertexData& V1, const UPbdVertexData& V2)
{
	const float D = (V1.InitialPosition - V2.InitialPosition).Size();
	return (V1.P - V2.P).Size() - D;
}

