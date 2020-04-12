// Fill out your copyright notice in the Description page of Project Settings.


#include "MusicFileAnalysis.h"

// Synethesia plugin stuff
#include "AudioAnalyzerNRT.h"
//#include "IAudioAnalyzerNRTInterface.h"
#include "AudioAnalyzerAsset.h"

#include "Components/AudioComponent.h"
#include "Sound/SoundWave.h"
#include "Misc/FileHelper.h" // Used to retrieve sound file from disk
#include "Interfaces/IAudioFormat.h"
#include "Runtime/Engine/Public/VorbisAudioInfo.h"

#include "Engine.h"

// Sets default values
AMusicFileAnalysis::AMusicFileAnalysis()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComponent->OnAudioPlaybackPercent.AddDynamic(this, &AMusicFileAnalysis::OnMusicPlayback);
	
	LoudnessNRT = CreateDefaultSubobject<ULoudnessNRT>("LoudnessNRT");
	ConstantQNRT = CreateDefaultSubobject<UConstantQNRT>("ConstantQNRT");
	OnsetNRT = CreateDefaultSubobject<UOnsetNRT>("OnsetNRT");
}

// Called when the game starts or when spawned
void AMusicFileAnalysis::BeginPlay()
{
	Super::BeginPlay();

	Test();
}

// Called every frame
void AMusicFileAnalysis::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

USoundWave* AMusicFileAnalysis::GetSoundWaveFromOggFile(const FString& file)
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

bool AMusicFileAnalysis::RetrieveSoundWaveInfoFromOggFile(USoundWave* SoundWav, TArray<uint8>* RawBinary) {

	// Wave Information Storage
	FSoundQualityInfo QualityInfo;
	FVorbisAudioInfo Vorbis_Object; // Vorbis is an audio coding format that generally provides the audio layer for the ogg file 

	// Reads .ogg header data into Vorbis_Object
	if (!Vorbis_Object.ReadCompressedInfo(RawBinary->GetData(), RawBinary->Num(), &QualityInfo))
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

void AMusicFileAnalysis::AddFrequencyBands()
{
	
}

void AMusicFileAnalysis::BeginVisualization()
{
	if (SoundWave)
	{
		LoudnessNRT->Sound = SoundWave;
		ConstantQNRT->Sound = SoundWave;
		OnsetNRT->Sound = SoundWave;

		AudioComponent->SetSound(LoudnessNRT->Sound);
		SoundWaveDuration = LoudnessNRT->Sound->Duration;

		AudioComponent->Play();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error: SoundWave file has not been set (MusicFileAanlysis.cpp)"));
	}
}

void AMusicFileAnalysis::OnMusicPlayback(const USoundWave* SoundWav, const float PlaybackPercent)
{
	TArray<float> Out;
	ConstantQNRT->GetNormalizedChannelConstantQAtTime(SoundWaveDuration * PlaybackPercent, 0, Out);
	//LoudnessNRT->GetNormalizedChannelLoudnessAtTime( (SoundWaveDuration * PlaybackPercent), 0, Out);
	UE_LOG(LogTemp, Error, TEXT("%f"), 1.0);
}

void AMusicFileAnalysis::RecieveFileString(const FString& File)
{
	SoundWave = GetSoundWaveFromOggFile(File);
}

void AMusicFileAnalysis::Test()
{
	BeginVisualization();
}

