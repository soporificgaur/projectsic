// Fill out your copyright notice in the Description page of Project Settings.

#include "MusicVisualizerGameMode.h"
#include "DefaultCameraPawn.h"
#include "UObject/ConstructorHelpers.h"

AMusicVisualizerGameMode::AMusicVisualizerGameMode()
{
	ConstructorHelpers::FClassFinder<ADefaultCameraPawn> DefaultCameraPawnBP(TEXT("/Game/DefaultCameraPawn"));
	if (!ensure(DefaultCameraPawnBP.Class != nullptr)) return;
	DefaultCameraPawnBPClass = DefaultCameraPawnBP.Class;

	DefaultPawnClass = DefaultCameraPawnBPClass;
}

/*void AMusicVisualizerGameMode::BeginVisualizationInit(const FString& File)
{
	// This method is overridded in MusicVisualizerGameMode_BP
}*/

void AMusicVisualizerGameMode::Visualize(float DeltaSeconds, TArray<float> LeftChannelSpectrumData, TArray<float> RightChannelSpectrumData, TArray<float> LeftChannelPeaks, TArray<float> RightChannlPeaks)
{

}

