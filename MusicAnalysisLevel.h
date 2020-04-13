// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "AudioAnalyzer/Classes/AudioAnalyzerManager.h"
#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MusicAnalysisLevel.generated.h"

/**
 * 
 */
UCLASS()
class MUSICVISUALIZER_API AMusicAnalysisLevel : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	AMusicAnalysisLevel();

	virtual void BeginPlay() override;

protected:
	//UAudioAnalyzerManager* AudioAnalyzerManager;
};
