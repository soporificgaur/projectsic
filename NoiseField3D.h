// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FastNoise.h"
#include "Particle.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/Material.h"
#include "Engine/StaticMeshActor.h"
#include "NoiseField3D.generated.h"

UCLASS()
class MUSICVISUALIZER_API ANoiseField3D : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANoiseField3D();

	//**********Audio Analysis Properties**********
	UPROPERTY(EditAnywhere)
	int NumBands = 8;
	UPROPERTY(EditAnywhere)
	bool SetParticleSpeedToMusic = true;
	UPROPERTY(EditAnywhere)
	bool SetParticleScaleToMusic = true;
	// How slow/fast the particles will move depending on music
	UPROPERTY(EditAnywhere)
	FVector2D MoveSpeedBounds = FVector2D(5.0f, 500.0f); 
	// How slow/fast the particles will rotate depending on music
	UPROPERTY(EditAnywhere)
	FVector2D RotateSpeedBounds = FVector2D(5.0f, 14.0f); 
	// If corresponding amplitude band does not pass this thresold, the particle will be black
	UPROPERTY(EditAnywhere)
	float AmplitudeMinThresholdColor = .35f;
	// Scale amplitude values so that the particles' brightness looks good
	UPROPERTY(EditAnywhere)
	float AmplitudeBrightnessMultiplier1 = .7f;
	// If corresponding amplitude band does not pass this thresold, the particle will not glow very brightly
	UPROPERTY(EditAnywhere)
	float AmplitudeMinThresholdGlow = .65f;
	UPROPERTY(EditAnywhere)
	float AmplitudeBrightnessMultiplier2 = 5.25f;
	// How fast the peak values for each band decay
	// Used in case a song has, for example, some kind of unique sound
	// that raises the peak of a frequency band. If this sound is never played again (or not played much)
	// the brightness of the particle would not reach its max again
	UPROPERTY(EditAnywhere)
	float PeakDecayTime = 30.0f;

	//  **********Noise Properties**********
	FastNoise* Noise;
	// Defines how large the vector grid will be (in terms of vectors)
	UPROPERTY(EditAnywhere)
	FIntVector NoiseGridSize = FIntVector(18, 26, 18); 
	// Defines noise resolution
	UPROPERTY(EditAnywhere)
	float NoiseIncrement = 6; 
	UPROPERTY(EditAnywhere)
	FVector Offset;
	// Determines how fast the noise field shifts
	UPROPERTY(EditAnywhere)
	FVector OffsetSpeed = FVector(12, 12, 12);
	// Defines how much space each vector influences
	UPROPERTY(EditAnywhere)
	float VectorSize = 26; 
	// Defines how many particles will be spawned
	UPROPERTY(EditAnywhere)
	int ParticleAmount = 1600; 
	UPROPERTY(EditAnywhere)
	//FVector2D ParticleSize = FVector2D(.03f, .12f);
	FVector2D ParticleSize = FVector2D(.03f, .08f);
	// Defines how closely particles can spawn
	UPROPERTY(EditAnywhere)
	float ParticleSpawnRadius = .04; 
	UPROPERTY(EditAnywhere)
	bool UseTanFuncForZNoise = false;


	// 3D array of FVectors
	// Its really a one dimensional array, but we can treat it like a 3D array using some multipliers
	TArray<FVector> VectorFlowDirections;
	
	UClass* NoiseParticle;

	UPROPERTY()
	bool UseGradient;

	UPROPERTY(EditAnywhere)
	TArray<UMaterial*> GradientMaterials;

	UPROPERTY(EditAnywhere)
	int GradientIndex = 0;

	UPROPERTY(EditAnywhere)
	AActor* ReflectiveSurface;

	UPROPERTY(EditAnywhere)
	bool UseReflectiveSurface = true;

protected:
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> ParticleDynamicMaterials;

	float ParticleMoveSpeed;
	float ParticleRotateSpeed;

	bool FinishedInit = false;

	TArray<float> LeftChannelSpectrumData;
	TArray<float> RightChannelSpectrumData;
	TArray<float> LeftChannelAverage;
	TArray<float> RightChannelAverage;

	TArray<float> LeftAudioAmplitude;
	TArray<float> RightAudioAmplitude;

	TArray<float> TotalBandAmplitude;
	TArray<float> PeakBandAmplitude;

	TArray<float> AmplitudeToVisualize;

	FVector ParticleNudgeDirection;

	float GridUpperXBound;
	float GridUpperYBound;
	float GridUpperZBound;

	float GridLowerXBound;
	float GridLowerYBound;
	float GridLowerZBound;

	TArray<AParticle*> NoiseFieldParticles;

	//UMaterialInterface* ParticleMaterial;

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

	void CheckForNewPeaks(float DeltaTime);
};
