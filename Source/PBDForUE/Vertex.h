// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vertex.generated.h"

class UParticleMeshComponent;

/**
 * 
 */
UCLASS()
class PBDFORUE_API UVertex : public UObject
{
	GENERATED_BODY()

public:
	bool bIsFixed;
	FVector X;
	FVector P;
	FVector V;
	float W;
	FVector InitialPosition;

	TObjectPtr<UParticleMeshComponent> ParticleMeshComponentRef;
};
