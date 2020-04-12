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