// Fill out your copyright notice in the Description page of Project Settings.

#include "NoiseField3D.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Engine/EngineTypes.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ANoiseField3D::ANoiseField3D()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<AActor> NoiseParticleClass(TEXT("/Game/NoiseParticle"));
	if (!ensure(NoiseParticleClass.Class != nullptr)) return;
	NoiseParticle = NoiseParticleClass.Class;
}

// Called when the game starts or when spawned
void ANoiseField3D::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorLocation( FVector( -(NoiseGridSize.X * VectorSize) / 2, -(NoiseGridSize.Y * VectorSize) / 2, -(NoiseGridSize.Z * VectorSize) / 2) );

	// Instantiate Noise
	Noise = new FastNoise();
	VectorFlowDirections.Init(FVector(0, 0, 0), NoiseGridSize.X * NoiseGridSize.Y * NoiseGridSize.Z);

	InitializeParticles();
	InitVisualization();
}

void ANoiseField3D::InitializeParticles()
{
	int SpawnAttempts = 0;
	for (int i = 0; i < ParticleAmount; i++)
	{
		// Limit the spawn attempts so that if the user tries to spawn too many particles,
		// and a suitable spawning location cannot be found, load will be taken off the cpu
		// by halting the spawning of more particles
		while (SpawnAttempts < 1000) {
			FVector RandomVec;
			// Create a random position to spawn the particle
			RandomVec = FVector(FMath::FRandRange(GetTransform().GetTranslation().X, GetTransform().GetTranslation().X + NoiseGridSize.X * VectorSize),
				FMath::FRandRange(GetTransform().GetTranslation().Y, GetTransform().GetTranslation().Y + NoiseGridSize.Y * VectorSize),
				FMath::FRandRange(GetTransform().GetTranslation().Z, GetTransform().GetTranslation().Z + NoiseGridSize.Z * VectorSize));

			bool CanSpawn = IsParticleSpawnValid(&RandomVec);
			if (CanSpawn)
			{
				AParticle* NewParticle = GetWorld()->SpawnActor<AParticle>(NoiseParticle, RandomVec, GetActorRotation());

				NewParticle->SetActorScale3D(FVector(ParticleSize));
				NewParticle->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				NoiseFieldParticles.Add(NewParticle);
				break;
			}
			++SpawnAttempts;
		}

		SpawnAttempts = 0;
	}
}

void ANoiseField3D::ApplyParticleMovement()
{
	GridUpperXBound = GetActorLocation().X + NoiseGridSize.X * VectorSize;
	GridUpperYBound = GetActorLocation().Y + NoiseGridSize.Y * VectorSize;
	GridUpperZBound = GetActorLocation().Z + NoiseGridSize.Z * VectorSize;

	GridLowerXBound = GetActorLocation().X;
	GridLowerYBound = GetActorLocation().Y;
	GridLowerZBound = GetActorLocation().Z;

	for (AParticle* particle : NoiseFieldParticles)
	{
		// Check that particles are not past the grid edges
		// Remember that the "location" of this actor is not in the center of the grid. Its in one of the bottom corners of the grid.

		if (particle->GetActorLocation().X > (GridUpperXBound))
		{
			particle->SetActorLocation(FVector(GridLowerXBound, particle->GetActorLocation().Y, particle->GetActorLocation().Z), false, nullptr, ETeleportType::TeleportPhysics);
			ParticleNudgeDirection = FVector::ForwardVector;
			particle->SetNudge();
		}
		else if (particle->GetActorLocation().X < (GridLowerXBound))
		{
			particle->SetActorLocation(FVector(GridUpperXBound, particle->GetActorLocation().Y, particle->GetActorLocation().Z), false, nullptr, ETeleportType::TeleportPhysics);
			ParticleNudgeDirection = -(FVector::ForwardVector);
			particle->SetNudge();
		}
		if (particle->GetActorLocation().Y > (GridUpperYBound))
		{
			particle->SetActorLocation(FVector(particle->GetActorLocation().X, GridLowerYBound, particle->GetActorLocation().Z), false, nullptr, ETeleportType::TeleportPhysics);
			ParticleNudgeDirection = FVector::RightVector;
			particle->SetNudge();
		}
		else if (particle->GetActorLocation().Y < (GridLowerYBound))
		{
			particle->SetActorLocation(FVector(particle->GetActorLocation().X, GridUpperYBound, particle->GetActorLocation().Z), false, nullptr, ETeleportType::TeleportPhysics);
			ParticleNudgeDirection = -(FVector::RightVector);
			particle->SetNudge();
		}
		if (particle->GetActorLocation().Z > (GridUpperZBound))
		{
			particle->SetActorLocation(FVector(particle->GetActorLocation().X, particle->GetActorLocation().Y, GridLowerZBound), false, nullptr, ETeleportType::TeleportPhysics);
			ParticleNudgeDirection = FVector::UpVector;
			particle->SetNudge();
		}
		else if (particle->GetActorLocation().Z < (GridLowerZBound))
		{
			particle->SetActorLocation(FVector(particle->GetActorLocation().X, particle->GetActorLocation().Y, GridUpperZBound), false, nullptr, ETeleportType::TeleportPhysics);
			ParticleNudgeDirection = -(FVector::UpVector);
			particle->SetNudge();
		}

		if (particle->GetNudge())
		{
			particle->SetNudgeDirection(ParticleNudgeDirection);
		}

		// Why clamp? If the particle goes out of bounds for some reason, we do not want the entire program to crash
		FVector ParticleGridPos = FVector(
		FMath::FloorToInt(FMath::Clamp(((particle->GetActorLocation().X - this->GetActorLocation().X) / VectorSize), 0.0f, (float)NoiseGridSize.X - 1)),
		FMath::FloorToInt(FMath::Clamp(((particle->GetActorLocation().Y - this->GetActorLocation().Y) / VectorSize), 0.0f, (float)NoiseGridSize.Y - 1)),
		FMath::FloorToInt(FMath::Clamp(((particle->GetActorLocation().Z - this->GetActorLocation().Z) / VectorSize), 0.0f, (float)NoiseGridSize.Z - 1)));
		particle->ApplyParticleRotation(VectorFlowDirections[ParticleGridPos.X + (NoiseGridSize.Y * ParticleGridPos.Y) + (NoiseGridSize.Z * NoiseGridSize.Y * ParticleGridPos.Z)].Rotation(), ParticleRotateSpeed);
		
		particle->SetMoveSpeed(ParticleModeSpeed);
		particle->SetActorScale3D(FVector(ParticleSize, ParticleSize, ParticleSize));
	}
}

// Called every frame
void ANoiseField3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Yes calling this every tick is not great for performance
	// But it looks pretty cool
	CalculateFlowVectors();
	ApplyParticleMovement();
	Visualize();
}

void ANoiseField3D::CalculateFlowVectors()
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	Offset = FVector(Offset.X + (OffsetSpeed.X * DeltaTime), Offset.Y + (OffsetSpeed.Y * DeltaTime), Offset.Z + (OffsetSpeed.Z * DeltaTime));

	// Generates a 3D field of vectors (but doesnt draw them)
	float xOffset = 0.0f;
	for (int x = 0; x < NoiseGridSize.X; x++)
	{
		float yOffset = 0.0f;
		for (int y = 0; y < NoiseGridSize.Y; y++)
		{
			float zOffset = 0.0f;
			for (int z = 0; z < NoiseGridSize.Z; z++)
			{
				// Simplex is the type of noise algorithm being used here.
				// Could generate noise in 4 dimensions but I am unsure what we would use the 4th dimension for 
				// Added 1 so that the returned float wont be negative 
				float GeneratedNoise = (Noise->GetSimplex(xOffset + Offset.X, yOffset + Offset.Y, zOffset + Offset.Z) + 1) / 2;

				// Generates a set of 3D vectors based on generated noise
				FVector NoiseDirection = FVector(FMath::Cos(GeneratedNoise * 2 * PI), FMath::Sin(GeneratedNoise * 2 * PI), FMath::Cos(GeneratedNoise * 2 * PI));
				//FVector Position = FVector(x, y, z) + GetTransform().GetTranslation();
				//FVector EndPosition = Position + NoiseDirection.GetSafeNormal();

				// Why get safe normal? In case vector is too small to safely normalize
				VectorFlowDirections[x + (NoiseGridSize.Y * y) + (NoiseGridSize.Z * NoiseGridSize.Y * z)] = NoiseDirection.GetSafeNormal();

				zOffset += NoiseIncrement;
			}
			yOffset += NoiseIncrement;
		}
		xOffset += NoiseIncrement;
	}
}

bool ANoiseField3D::IsParticleSpawnValid(FVector* Position)
{
	for (AParticle* particle : NoiseFieldParticles)
	{
		if (FVector::Dist(*Position, particle->GetTransform().GetTranslation()) < .05) {
			return false;
		}
	}
	return true;
}

void ANoiseField3D::InitVisualization()
{
	// Divide the particles up evenly into bands so that each frequency band
	// is represented equally by a set of particles
	// You could change this to a logarithmic division of particles if you wanted
	// more particles to represent a specific frequency
	int Counter = 0;
	for (AParticle* particle : NoiseFieldParticles)
	{
		particle->SetFrequencyBand((Counter % NumBands));
		++Counter;
	}
}

void ANoiseField3D::RecieveAudioInformation(TArray<float> LCSData, TArray<float> RCSData, TArray<float> LCA, TArray<float> RCA, TArray<float> LAA, TArray<float> RAA)
{
	LeftChannelSpectrumData = LCSData;
	RightChannelSpectrumData = RCSData;
	LeftChannelAverage = LCA;
	RightChannelAverage = RCA;

	LeftAudioAmplitude = LAA;
	RightAudioAmplitude = RAA;
}

void ANoiseField3D::Visualize()
{
	if (SetParticleSpeedToMusic && FinishedInit)
	{
		// For the love of god dont change these indicies or the entire program will crash
		float TotalAudioAmplitude = LeftAudioAmplitude[0] + RightAudioAmplitude[0];
		float Speed = FMath::Lerp(MoveSpeedBounds.X, MoveSpeedBounds.Y, TotalAudioAmplitude);
		ParticleModeSpeed = Speed;

		UE_LOG(LogTemp, Warning, TEXT("%f"), TotalAudioAmplitude);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Not Visualized"));
}

// You have to make sure everything is finished initializing before we start the visualization
void ANoiseField3D::FinishInit()
{
	GEngine->AddOnScreenDebugMessage(-10, 1.f, FColor::Yellow, TEXT("InitFinished"));
	FinishedInit = true;
}



