// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MusicVisualizerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MUSICVISUALIZER_API AMusicVisualizerGameMode : public AGameModeBase
{
	GENERATED_BODY()

	AMusicVisualizerGameMode();

	TSubclassOf<class ADefaultCameraPawn> DefaultCameraPawnBPClass;
};
