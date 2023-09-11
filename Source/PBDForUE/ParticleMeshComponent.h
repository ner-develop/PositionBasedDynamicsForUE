// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/MeshComponent.h"
#include "ParticleMeshComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class PBDFORUE_API UParticleMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	int32 GetAlignmentOrder() const { return AlignmentOrder; }
	float GetMass() const { return Mass; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	float Mass = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	int32 AlignmentOrder = -1;
};
