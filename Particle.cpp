// Fill out your copyright notice in the Description page of Project Settings.

#include "Particle.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h" 
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
AParticle::AParticle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ParticleMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("ParticleMesh"));
}

// Called when the game starts or when spawned
void AParticle::BeginPlay()
{
	Super::BeginPlay();

	ParticleMeshComponent->SetStaticMesh(ParticleMesh);
	ParticleMeshComponent->SetWorldScale3D(FVector(GetActorScale3D().X * ScaleMultipliers.X, GetActorScale3D().Y * ScaleMultipliers.Y, GetActorScale3D().Z * ScaleMultipliers.Z));
}

// Called every frame
void AParticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Consider moving this elsewhere so that 
	// 1000 objects do not have to tick
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * MoveSpeed * DeltaTime);

	if (RenderDisabledFrameCount > 0)
	{
		RenderDisabledFrameCount++;
		// Do not render particles for X frames after they are teleported from reaching the grid boundry
		// Might reduce visual pop-in
		if(RenderDisabledFrameCount > 4)
		{
			ParticleMeshComponent->SetVisibility(true);
			RenderDisabledFrameCount = 0;
		}
	}
}

void AParticle::ApplyParticleRotation(FRotator Rotation, float RotateSpeed)
{
	// Use quaternions because they are not subject to gimbal lock
	FQuat CurrentRotQuat = FQuat(GetActorRotation());

	if (ParticleNudge)
	{
		ParticleNudgeTimer = ParticleNudgeDefault;
		ParticleNudge = false;
	}
	if (ParticleNudgeTimer > 0)
	{
		ParticleNudgeTimer -= GetWorld()->GetDeltaSeconds();

		FVector TargetDir = (Rotation.Vector() + ParticleNudgeDirection) / 2;
		FQuat TargetRotQuat = FQuat(TargetDir.Rotation());

		SetActorRotation(FQuat::Slerp(CurrentRotQuat, TargetRotQuat, GetWorld()->GetDeltaSeconds() * RotateSpeed * ParticleNudgeRotateSpeedMultiplier));
	}
	else
	{
		FQuat TargetRotQuat = FQuat(Rotation);
		SetActorRotation(FQuat::Slerp(CurrentRotQuat, TargetRotQuat, GetWorld()->GetDeltaSeconds() * RotateSpeed));
	}
}

void AParticle::SetParticleScale(float Scale)
{
	ParticleScale = Scale;
}

void AParticle::ApplyParticleScale()
{
	SetActorScale3D(FVector(ParticleScale * ScaleMultipliers.X, ParticleScale * ScaleMultipliers.Y, ParticleScale * ScaleMultipliers.Z));
}

void AParticle::SetNudgeDirection(FVector Dir)
{
	ParticleNudgeDirection = Dir;
}

void AParticle::SetNudge()
{
	ParticleNudge = true;
}

bool AParticle::GetNudge()
{
	return ParticleNudge;
}

void AParticle::SetMoveSpeed(float MS)
{
	MoveSpeed = MS;
}

void AParticle::SetFrequencyBand(int Band)
{
	FrequencyBandIndex = Band;
}

int AParticle::GetFrequencyBandIndex()
{
	return FrequencyBandIndex;
}

UMaterialInterface* AParticle::GetMaterial()
{
	return ParticleMeshComponent->GetMaterial(0);
}

void AParticle::SetMaterial(UMaterialInterface* Material)
{
	ParticleMeshComponent->SetMaterial(0, Material);
}

void AParticle::DelayRender()
{
	ParticleMeshComponent->SetVisibility(false);
	RenderDisabledFrameCount = 1;
}