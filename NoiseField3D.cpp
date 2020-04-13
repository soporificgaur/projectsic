// Fill out your copyright notice in the Description page of Project Settings.

#include "NoiseField3D.h"
#include "Engine/World.h"
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
	
	// Instantiate Noise
	Noise = new FastNoise();
	VectorFlowDirections.Init(FVector(0, 0, 0), NoiseGridSize.X * NoiseGridSize.Y * NoiseGridSize.Z);

	InitializeParticles();
	DrawNoiseGridSimplex();
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
				AActor* NewParticle = GetWorld()->SpawnActor<AActor>(NoiseParticle, RandomVec, GetActorRotation());

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

// Called every frame
void ANoiseField3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Yes calling this every tick is not great for performance
	// But it looks pretty cool
	CalculateFlowVectors();
}

void ANoiseField3D::CalculateFlowVectors()
{
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
				float GeneratedNoise = (Noise->GetSimplex(xOffset, yOffset, zOffset) + 1) / 2;

				// Generates a set of 3D vectors based on generated noise
				FVector NoiseDirection = FVector(FMath::Cos(GeneratedNoise * 2 * PI), FMath::Sin(GeneratedNoise * 2 * PI), FMath::Cos(GeneratedNoise * 2 * PI));
				//FVector Position = FVector(x, y, z) + GetTransform().GetTranslation();
				//FVector EndPosition = Position + NoiseDirection.GetSafeNormal();

				// Why get safe normal? In case vector is too small to safely normalize
				//VectorFlowDirections[x][y][z] = NoiseDirection.GetSafeNormal();
				zOffset += NoiseResolution;
			}
			yOffset += NoiseResolution;
		}
		xOffset += NoiseResolution;
	}
}

bool ANoiseField3D::IsParticleSpawnValid(FVector* Position)
{
	for (AActor* particle : NoiseFieldParticles)
	{
		if (FVector::Dist(*Position, particle->GetTransform().GetTranslation()) < .05) {
			return false;
		}
	}
	return true;
}

void ANoiseField3D::DrawNoiseGridSimplex()
{
	
}

// Remember to delete noise pointer

