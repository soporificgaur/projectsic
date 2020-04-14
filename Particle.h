// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particle.generated.h"

UCLASS()
class MUSICVISUALIZER_API AParticle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParticle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int FrequencyBandIndex;

	float MoveSpeed = 1.0f;

	// If the particle has recently crossed the grid border,
	// we want to nudge it away from the border to which the particle teleports
	// to prevent a particle from continuously teleporting
	// when it is stuck in a vector field along a border
	UPROPERTY(EditAnywhere)
	float ParticleNudgeRotateSpeedMultiplier = 2.0f;
	float ParticleNudgeDefault = .5f;
	float ParticleNudgeTimer = .5f;
	bool ParticleNudge = false;
	FVector ParticleNudgeDirection;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ApplyParticleRotation(FRotator Rotation, float RotateSpeed);

	void SetMoveSpeed(float MS);

	void SetNudge();

	bool GetNudge();

	void SetNudgeDirection(FVector Dir);

	void SetFrequencyBand(int Band);
};
