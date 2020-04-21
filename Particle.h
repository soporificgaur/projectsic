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

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UStaticMeshComponent* ParticleMeshComponent;

	UPROPERTY(EditAnywhere)
	UStaticMesh* ParticleMesh;

	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditAnywhere)
	FVector ScaleMultipliers = FVector(2.5f, .75f, .75f);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int FrequencyBandIndex;

	float MoveSpeed = 1.0f;

	int RenderDisabledFrameCount = 0;

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

	float ParticleScale = .03f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ApplyParticleRotation(FRotator Rotation, float RotateSpeed);

	void ApplyParticleScale();

	void SetMoveSpeed(float MS);

	void SetNudge();

	bool GetNudge();

	void SetNudgeDirection(FVector Dir);

	void SetFrequencyBand(int Band);

	void DelayRender();

	void SetParticleScale(float Scale);

	int GetFrequencyBandIndex();

	class UMaterialInterface* GetMaterial();

	void SetMaterial(UMaterialInterface* Material);
};
