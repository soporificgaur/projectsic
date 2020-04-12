// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MusicAnalysisInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMusicAnalysisInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MUSICVISUALIZER_API IMusicAnalysisInterface
{
	GENERATED_BODY()

public:

	virtual void RecieveFileString(const FString& File) = 0;

	virtual void BeginVisualization() = 0;
};
