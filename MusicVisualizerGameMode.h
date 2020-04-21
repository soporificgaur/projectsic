// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Runtime/Engine/Public/VorbisAudioInfo.h"
#include "MusicAnalysisInterface.h"
#include "ParticleFieldInterface.h"
#include "MusicVisualizerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MUSICVISUALIZER_API AMusicVisualizerGameMode : public AGameModeBase, public IMusicAnalysisInterface, public IParticleFieldInterface
{
	GENERATED_BODY()

	AMusicVisualizerGameMode();

	TSubclassOf<class ADefaultCameraPawn> DefaultCameraPawnBPClass;
	class ANoiseField3D* ANoiseFieldPtr;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void BeginVisualizationInit(const FString& File);

	UFUNCTION(BlueprintCallable)
	void Visualize(TArray<float> LeftChannelSpectrumData, TArray<float> RightChannelSpectrumData,
		TArray<float> LeftChannelAverage, TArray<float> RightChannelAverage,
		TArray<float> LeftAudioAmplitude, TArray<float> RightAudioAmplitude);

	// Called when the game starts or when spawned
	void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void FinishInit();

protected:
	bool FinishedInit = false;

	// Use to change things in c++ after it was changed in the editor (currently unused)
	//virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// Delegate thats called by AudioComponent
	//UFUNCTION()
	//void OnMusicPlayback(const USoundWave* SoundWav, const float PlaybackPercent);

	class UAudioComponent* AudioComponent;

	float SoundWaveDuration;

	class AMusicVisualizerGameMode* GameMode;

public:
	// Sound wave to do analysis with
	UPROPERTY(EditAnywhere)
	FString SoundWaveString;

	void Test();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Why from ogg file? I've read that unreal engine converts everything to an ogg file during runtime to have a consistant audio format
	// It may be speculation, but from my research it is much easier to retrieve ogg files during runtime than wave files.
	// The working methods I have seen to retrieve wave files involve reading in values from the wave file header
	// using hardcoded positions to index into an array of binary. 
	class USoundWave* GetSoundWaveFromOggFile(const FString& file);

	bool RetrieveSoundWaveInfoFromOggFile(USoundWave* SoundWav, TArray<uint8>* RawBinary);

	UFUNCTION(BlueprintCallable)
	// Recieve the string to retrieve .ogg sound file with 
	void RecieveFileString(const FString& File) override;

	UFUNCTION(BlueprintCallable)
	// Recieve request to begin the visualization
	void BeginVisualization() override;

	UFUNCTION(BlueprintCallable)
	void SetMoveSpeedMin(float Speed) override;

	UFUNCTION(BlueprintCallable)
	void SetMoveSpeedMax(float Speed) override;

	UFUNCTION(BlueprintCallable)
	void SetRotateSpeedMin(float Speed) override;

	UFUNCTION(BlueprintCallable)
	void SetRotateSpeedMax(float Speed) override;

	UFUNCTION(BlueprintCallable)
	void SetAmplitudeBrightnessMultiplier1(float Multiplier) override;

	UFUNCTION(BlueprintCallable)
	void SetAmplitudeBrightnessMultiplier2(float Multiplier) override;

	UFUNCTION(BlueprintCallable)
	void SetParticleAmount(int Amount) override;

	UFUNCTION(BlueprintCallable)
	void SetParticleSizeMin(float Size) override;

	UFUNCTION(BlueprintCallable)
	void SetParticleSizeMax(float Size) override;

	UFUNCTION(BlueprintCallable)
	void SetGradientIndex(int Index) override;

	UFUNCTION(BlueprintCallable)
	void UseTanForZNoise(bool UseTan) override;
};
