// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Synesthesia plugin stuff
#include "LoudnessNRT.h"
#include "ConstantQNRT.h"
#include "OnsetNRT.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MusicAnalysisInterface.h"
#include "MusicFileAnalysis.generated.h"

UCLASS()
class MUSICVISUALIZER_API AMusicFileAnalysis : public AActor, public IMusicAnalysisInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMusicFileAnalysis();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Use to change things in c++ after it was changed in the editor (currently unused)
	//virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// Add 24 frequency bands to analyze 
	// Based on Bark scale critical bands
	void AddFrequencyBands();

	// Delegate thats called by AudioComponent
	UFUNCTION()
	void OnMusicPlayback(const USoundWave* SoundWav, const float PlaybackPercent);

	class UAudioComponent* AudioComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ULoudnessNRT* LoudnessNRT;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UConstantQNRT* ConstantQNRT;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UOnsetNRT* OnsetNRT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SoundWaveDuration;

	float SoundClipDuration;


public:	
	UPROPERTY(EditAnywhere)
	// Sound wave to do analysis with
	USoundWave* SoundWave;

	void Test();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Why from ogg file? I've heard that unreal engine converts everything to an ogg file during runtime to have a consistant audio format
	// It may be speculation, but from my research it is much easier to retrieve ogg files during runtime than wave files.
	// The working methods I have seen to retrieve wave files involve reading in values from the wave file header
	// using hardcoded positions to index into an array of binary. 
	class USoundWave* GetSoundWaveFromOggFile(const FString& file);

	bool RetrieveSoundWaveInfoFromOggFile(USoundWave* SoundWav, TArray<uint8>* RawBinary);

	// Recieve the string to retrieve .ogg sound file with 
	virtual void RecieveFileString(const FString& File) override;

	// Recieve request to begin the visualization
	virtual void BeginVisualization() override;
};
