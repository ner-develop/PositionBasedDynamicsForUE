// Fill out your copyright notice in the Description page of Project Settings.


#include "Rope.h"

// Sets default values
ARope::ARope()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARope::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARope::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

