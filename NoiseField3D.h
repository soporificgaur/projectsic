// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FastNoise.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NoiseField3D.generated.h"

UCLASS()
class MUSICVISUALIZER_API ANoiseField3D : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANoiseField3D();

	// Noise Properties
	FastNoise* Noise;
	UPROPERTY(EditAnywhere)
	FIntVector NoiseGridSize = FIntVector(20, 20, 20); // Defines how large the vector grid will be (in terms of vectors)
	UPROPERTY(EditAnywhere)
	float NoiseResolution = 5; // Defines noise resolution
	UPROPERTY(EditAnywhere)
	FVector Offset;
	UPROPERTY(EditAnywhere)
	FVector OffsetSpeed;
	UPROPERTY(EditAnywhere)
	float VectorSize = 30; // Defines how much space each vector influences
	UPROPERTY(EditAnywhere)
	int ParticleAmount = 1000; // Defines how many particles will be spawned
	UPROPERTY(EditAnywhere)
	float ParticleSize = .1;
	UPROPERTY(EditAnywhere)
	float ParticleSpawnRadius = .05; // Defines how closely particles can spawn

	// 3D array of FVectors
	// Its really a one dimensional array,
	// but using some big brain mode we can treat it like a 3D array
	TArray<FVector> VectorFlowDirections;

	UClass* NoiseParticle;

protected:
	TArray<AActor*> NoiseFieldParticles;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Calculate field of 3D vectors based on random generated noise
	// Much performance can be gained by calculating this in a shader instead
	// since it would use the GPU rather than the CPU, and the GPU
	// is much more performant for tasks involving numerous float calculations that
	// can be done in parallel. But since we only have a week left, I will move this
	// to a shader if we finish fast enough
	void CalculateFlowVectors();

	void DrawNoiseGridSimplex();

	void InitializeParticles();

	// Checks if the particle is spawning DEEP inside another particle
	// Returns true if the particle is not inside another particle
	// This allows us to spread our particles so that we can achieve 
	// The appearance of as many particles as possible without having to spawn too many
	bool IsParticleSpawnValid(FVector* Position);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
