// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DefaultCameraPawn.generated.h"

UCLASS()
class MUSICVISUALIZER_API ADefaultCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADefaultCameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RotateFocalPoint(float DeltaTime);

	FRotator FromPitch;
	FRotator ToPitch;  

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetFocalPoint(class USceneComponent* FocalPointComponent);

	// Note: These values are multiplied where used by values that allow
	// the variables set here to seem like "good values" at a default of 1
	UPROPERTY(EditAnywhere)
	float HorizontalSpeed = 1.0f;
	UPROPERTY(EditAnywhere)
	float VerticalSpeed = 1.0f;
	UPROPERTY(EditAnywhere)
	float VerticalAmplitude = 1.0f;
	UPROPERTY(EditAnywhere)
	USceneComponent* FocalPoint;

};
