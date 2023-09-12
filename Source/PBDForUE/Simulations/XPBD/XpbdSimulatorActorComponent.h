// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "XpbdSimulatorActorComponent.generated.h"

class UXpbdParticleData;
class UXpbdConstraint;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PBDFORUE_API UXpbdSimulatorActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UXpbdSimulatorActorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleInstanceOnly)
	TArray<UXpbdParticleData*> ParticleArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	bool bIsEnabledSimulation = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	double Flexibility = 0.0001f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	int32 Iterations = 10;

private:
	const FVector Gravity = FVector(0, 0, -98.f);

	UPROPERTY()
	TArray<UXpbdConstraint*> ConstraintArray;

	void Step(double Δt);
	void Simulate(double Δt);
};
