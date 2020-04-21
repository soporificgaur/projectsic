// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ParticleFieldInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UParticleFieldInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MUSICVISUALIZER_API IParticleFieldInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SetMoveSpeedMin(float Speed) = 0;

	virtual void SetMoveSpeedMax(float Speed) = 0;

	virtual void SetRotateSpeedMin(float Speed) = 0;

	virtual void SetRotateSpeedMax(float Speed) = 0;

	virtual void SetAmplitudeBrightnessMultiplier1(float Multiplier) = 0;

	virtual void SetAmplitudeBrightnessMultiplier2(float Multiplier) = 0;

	virtual void SetParticleAmount(int Amount) = 0;

	virtual void SetParticleSizeMin(float Size) = 0;
	
	virtual void SetParticleSizeMax(float Size) = 0;

	virtual void SetGradientIndex(int Index) = 0;

	virtual void UseTanForZNoise(bool UseTan) = 0;
};
