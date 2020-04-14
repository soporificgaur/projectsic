// Fill out your copyright notice in the Description page of Project Settings.

#include "MusicVisualizerGameMode.h"
#include "NoiseField3D.h"
#include "EngineUtils.h"
#include "GameFramework/GameUserSettings.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
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

void AMusicVisualizerGameMode::Visualize(TArray<float> LeftChannelSpectrumData, TArray<float> RightChannelSpectrumData, TArray<float> LeftChannelAverage, TArray<float> RightChannelAverage, TArray<float> LeftAudioAmplitude, TArray<float> RightAudioAmplitude)
{
	if (ANoiseFieldPtr)
	{
		ANoiseFieldPtr->RecieveAudioInformation(LeftChannelSpectrumData, RightChannelSpectrumData, LeftChannelAverage, RightChannelAverage, LeftAudioAmplitude, RightAudioAmplitude);
	}
}

void AMusicVisualizerGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameUserSettings* UserSettings = GetGameInstance()->GetEngine()->GetGameUserSettings();
	UserSettings->SetFrameRateLimit(59);

	for (TActorIterator<ANoiseField3D> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ANoiseFieldPtr = Cast<ANoiseField3D>(*ActorItr);
		if (ANoiseFieldPtr)
		{
			break;
		}
	}
}

void AMusicVisualizerGameMode::FinishInit()
{
	ANoiseFieldPtr->FinishInit();
}

