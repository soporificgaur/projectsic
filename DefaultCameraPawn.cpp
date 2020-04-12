// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultCameraPawn.h"
#include "Components/SceneComponent.h"
#include "Engine.h"

// Sets default values
ADefaultCameraPawn::ADefaultCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called to bind functionality to input
void ADefaultCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Called when the game starts or when spawned
void ADefaultCameraPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADefaultCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateFocalPoint(DeltaTime);
}

void ADefaultCameraPawn::SetFocalPoint(USceneComponent* FocalPointComponent)
{
	FocalPoint = FocalPointComponent;
}

void ADefaultCameraPawn::RotateFocalPoint(float DeltaTime)
{
	if (FocalPoint) {

		float Time = GetWorld()->GetRealTimeSeconds();

		FromPitch = FRotator(15.0f * VerticalAmplitude, 0, 0);
		ToPitch = FRotator(-15.0f * VerticalAmplitude, 0, 0);

		FQuat FromQuatPitch = FQuat(FromPitch);
		FQuat ToQuatPitch = FQuat(ToPitch);

		// Need to add 1 and multiply by .5 so that the value of slerp ranges from 0 - 1 (no negative, or else
		// the amplitude of the camera's vertical motion will double)
		float Slerp = 0.5f * (1 + FMath::Sin(PI * Time * (VerticalSpeed / 4) ));

		FQuat QuatYaw = FQuat(FRotator(0, FocalPoint->GetComponentRotation().Yaw + (DeltaTime * (HorizontalSpeed * 10) ), 0));
		FQuat QuatPitch = FQuat::Slerp(FromQuatPitch, ToQuatPitch, Slerp);

		FocalPoint->SetWorldRotation(QuatYaw.Rotator() + QuatPitch.Rotator());
	}
}
