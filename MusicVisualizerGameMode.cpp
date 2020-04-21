// Fill out your copyright notice in the Description page of Project Settings.

#include "MusicVisualizerGameMode.h"
#include "NoiseField3D.h"
#include "EngineUtils.h"
#include "GameFramework/GameUserSettings.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "DefaultCameraPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWave.h"
#include "Misc/FileHelper.h" // Used to retrieve sound file from disk
#include "Interfaces/IAudioFormat.h"


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

	Test();
}

void AMusicVisualizerGameMode::Test()
{
	SoundWaveString = "Test2.ogg";
	BeginVisualization();
}

void AMusicVisualizerGameMode::BeginVisualization()
{
	BeginVisualizationInit(SoundWaveString);
}

void AMusicVisualizerGameMode::RecieveFileString(const FString& File)
{
	//SoundWave = GetSoundWaveFromOggFile(File);
	SoundWaveString = File;
}

void AMusicVisualizerGameMode::FinishInit()
{
	FinishedInit = true;
	ANoiseFieldPtr->FinishInit();
}


// Called every frame
void AMusicVisualizerGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMusicVisualizerGameMode::SetMoveSpeedMin(float Speed)
{
	ANoiseFieldPtr->MoveSpeedBounds = FVector2D(Speed, ANoiseFieldPtr->MoveSpeedBounds.Y);
}

void AMusicVisualizerGameMode::SetMoveSpeedMax(float Speed)
{
	ANoiseFieldPtr->MoveSpeedBounds = FVector2D(ANoiseFieldPtr->MoveSpeedBounds.X, Speed);
}

void AMusicVisualizerGameMode::SetRotateSpeedMin(float Speed)
{
	ANoiseFieldPtr->RotateSpeedBounds = FVector2D(Speed, ANoiseFieldPtr->RotateSpeedBounds.Y);
}

void AMusicVisualizerGameMode::SetRotateSpeedMax(float Speed)
{
	ANoiseFieldPtr->RotateSpeedBounds = FVector2D(ANoiseFieldPtr->RotateSpeedBounds.X, Speed);
}

void AMusicVisualizerGameMode::SetAmplitudeBrightnessMultiplier1(float Multiplier)
{
	ANoiseFieldPtr->AmplitudeBrightnessMultiplier1 = Multiplier;
}

void AMusicVisualizerGameMode::SetAmplitudeBrightnessMultiplier2(float Multiplier)
{
	ANoiseFieldPtr->AmplitudeBrightnessMultiplier2 = Multiplier;
}

void AMusicVisualizerGameMode::SetParticleAmount(int Amount)
{
	ANoiseFieldPtr->ParticleAmount = Amount;
}

void AMusicVisualizerGameMode::SetParticleSizeMin(float Size)
{
	ANoiseFieldPtr->ParticleSize = FVector2D(Size, ANoiseFieldPtr->ParticleSize.Y);
}

void AMusicVisualizerGameMode::SetParticleSizeMax(float Size)
{
	ANoiseFieldPtr->ParticleSize = FVector2D(ANoiseFieldPtr->ParticleSize.X, Size);
}

void AMusicVisualizerGameMode::SetGradientIndex(int Index)
{
	ANoiseFieldPtr->GradientIndex = Index;
}

void AMusicVisualizerGameMode::UseTanForZNoise(bool UseTan)
{
	ANoiseFieldPtr->UseTanFuncForZNoise = UseTan;
}

// These methods to create a sound wave object from an ogg file are currently unused since the music visualization plugin implements a version that works for more than ogg file types,
// but these methods may be beneficial in the future for creating a more customizable file retrieve system
USoundWave* AMusicVisualizerGameMode::GetSoundWaveFromOggFile(const FString& file)
{
	// Tells if loading the file was a success
	bool LoadedFile;
	// Tells if loading the sound wave was a success
	bool LoadedSoundWave = false;

	USoundWave* SoundWav = NewObject<USoundWave>(USoundWave::StaticClass());

	// Make sure SoundWav is initialized
	if (!ensure(SoundWav != nullptr)) {
		delete SoundWav; // Free pointer
		return nullptr;
	}

	// Will store the binary sound file in here. Use uint8 to save memory (since values will only be in the range of 0-1)
	TArray<uint8> BinarySoundFile;

	// Use static class FFileHelper to retrieve the soud file.
	// the .GetData() method is used to retrieve a const wchar_t* from the TArray<wchar_t>
	// (the function declaration of LoadFileToArray takes a TCHAR*, but TCHAR is just an alias for wchar_t
	LoadedFile = FFileHelper::LoadFileToArray(BinarySoundFile, file.GetCharArray().GetData());

	if (LoadedFile) {
		// Get the byte data in the format of a .ogg file
		FByteBulkData* data = &(SoundWav->CompressedFormatData.GetFormat(TEXT("OGG")));

		// Make read-write operations allowed on returned memory.
		data->Lock(LOCK_READ_WRITE);

		// Copy the contents of the raw sound file "binarySoundFile" to the FByteBulkData variable "data"
		// Method Realloc only works if the data has been locked
		FMemory::Memcpy(data->Realloc(BinarySoundFile.Num()), BinarySoundFile.GetData(), BinarySoundFile.Num());

		data->Unlock();

		if (RetrieveSoundWaveInfoFromOggFile(SoundWav, &BinarySoundFile))
			LoadedSoundWave = true;
		else
			LoadedSoundWave = false;
	}

	if (LoadedSoundWave) {
		return SoundWav;
	}
	else {
		return nullptr;
		delete SoundWav;
	}
}

bool AMusicVisualizerGameMode::RetrieveSoundWaveInfoFromOggFile(USoundWave* SoundWav, TArray<uint8>* RawBinary) {

	// Wave Information Storage
	FSoundQualityInfo QualityInfo;
	// Vorbis is an audio coding format that generally provides the audio layer for the ogg file 
	FVorbisAudioInfo Vorbis_Obj;

	// Reads .ogg header data into Vorbis_Obj
	if (!Vorbis_Obj.ReadCompressedInfo(RawBinary->GetData(), RawBinary->Num(), &QualityInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("Error: The ogg header cannot be read."));
		return 0;
	}

	if (!ensure(SoundWav != nullptr)) return 0;

	SoundWav->SoundGroup = ESoundGroup::SOUNDGROUP_Default;

	// Read data from the header of .ogg file into our USoundWav object. Essentially used to convert the ogg file to a wav file
	SoundWav->NumChannels = QualityInfo.NumChannels;
	SoundWav->Duration = QualityInfo.Duration;
	SoundWav->RawPCMDataSize = QualityInfo.SampleDataSize;
	SoundWav->SetSampleRate(QualityInfo.SampleRate);

	return 1;
}

