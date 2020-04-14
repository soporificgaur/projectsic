// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FastNoise.h"
#include "Particle.h"
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
	
	//Audio Analysis Properties
	UPROPERTY(EditAnywhere)
	int NumBands = 8;
	UPROPERTY(EditAnywhere)
	bool SetParticleSpeedToMusic = true;
	UPROPERTY(EditAnywhere)
	FVector2D MoveSpeedBounds = FVector2D(50.0f, 700.0f); // How slow/fast the particles will move depending on music
	UPROPERTY(EditAnywhere)
	FVector2D RotateSpeedBounds = FVector2D(.5f, 2.5f); // How slow/fast the particles will rotate depending on music

	// Noise Properties
	FastNoise* Noise;
	// Defines how large the vector grid will be (in terms of vectors)
	UPROPERTY(EditAnywhere)
	FIntVector NoiseGridSize = FIntVector(20, 20, 20); 

	// Defines noise resolution
	UPROPERTY(EditAnywhere)
	float NoiseIncrement = 5; 

	UPROPERTY(EditAnywhere)
	FVector Offset;

	// Determines how fast the noise field shifts
	UPROPERTY(EditAnywhere)
	FVector OffsetSpeed = FVector(10, 10, 10); 

	// Defines how much space each vector influences
	UPROPERTY(EditAnywhere)
	float VectorSize = 70; 

	// Defines how many particles will be spawned
	UPROPERTY(EditAnywhere)
	int ParticleAmount = 1750; 

	UPROPERTY(EditAnywhere)
	float ParticleSize = .07;

	// Defines how closely particles can spawn
	UPROPERTY(EditAnywhere)
	float ParticleSpawnRadius = .05; 

	UPROPERTY(EditAnywhere)
	float ParticleModeSpeed = 125.0f;

	// Defines how quickly the particles will rotate to their corresponding vector 
	UPROPERTY(EditAnywhere)
	float ParticleRotateSpeed = 1.25f; 

	// 3D array of FVectors
	// Its really a one dimensional array,
	// but using some big brain mode we can treat it like a 3D array
	TArray<FVector> VectorFlowDirections;
	
	UClass* NoiseParticle;

protected:
	bool FinishedInit = false;

	TArray<float> LeftChannelSpectrumData;
	TArray<float> RightChannelSpectrumData;
	TArray<float> LeftChannelAverage;
	TArray<float> RightChannelAverage;

	TArray<float> LeftAudioAmplitude;
	TArray<float> RightAudioAmplitude;

	FVector ParticleNudgeDirection;

	float GridUpperXBound;
	float GridUpperYBound;
	float GridUpperZBound;

	float GridLowerXBound;
	float GridLowerYBound;
	float GridLowerZBound;

	TArray<AParticle*> NoiseFieldParticles;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Calculate field of 3D vectors based on random generated noise
	// Much performance can be gained by calculating this in a shader instead
	// since it would use the GPU rather than the CPU, and the GPU
	// is much more performant for tasks involving numerous float calculations that
	// can be done in parallel. But since we only have a week left, I will move this
	// to a shader if we finish fast enough
	void CalculateFlowVectors();

	void InitializeParticles();

	// Checks if the particle is spawning DEEP inside another particle
	// Returns true if the particle is not inside another particle
	// This allows us to spread our particles so that we can achieve 
	// The appearance of as many particles as possible without having to spawn too many
	bool IsParticleSpawnValid(FVector* Position);

	void ApplyParticleMovement();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void RecieveAudioInformation(TArray<float> LCSData, TArray<float> RCSData, TArray<float> LCP, TArray<float> RCP, TArray<float> LeftAudioAmplitude, TArray<float> RightAudioAmplitude);

	void Visualize();

	void InitVisualization();

	void FinishInit();
};
