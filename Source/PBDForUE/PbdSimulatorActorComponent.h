// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PbdSimulatorActorComponent.generated.h"


class UParticleMeshComponent;
class UVertex;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PBDFORUE_API UPbdSimulatorActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPbdSimulatorActorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleInstanceOnly)
	TArray<UVertex*> VertexArray;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameters")
	float Stiffness = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameters")
	int32 Iterations = 10;


private:
	const FVector Gravity = FVector(0, 0, -98.f);
	void Step(const float DeltaTime);
	void Simulate(const float DeltaTime);
	void ProjectConstraints(const float K);
	static float Constraint(const UVertex& V1, const UVertex& V2);
};
