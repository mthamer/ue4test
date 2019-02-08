// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "test1GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

AMyPlayerController::AMyPlayerController()
{
//	UE_LOG(LogTemp, Warning, TEXT("MOOSE: AMyPlayerController ctor"));

	// set these for CalcCamera to be called
	PrimaryActorTick.bCanEverTick = true;
	bAutoManageActiveCameraTarget = false;
	mFlockCamera = nullptr;
}

void AMyPlayerController::BeginPlay()
{
	// search for FlockCamera actor, TODO - expose this in editor
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundActors);

	mFlockCamera = nullptr;
	for (int i = 0; i < FoundActors.Num(); i++)
	{
		if (FoundActors[i]->GetName() == "FlockCamera")
		{
			mFlockCamera = Cast<ACameraActor>(FoundActors[i]);
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("MOOSE: Couldn't find FlockCamera"));
}

void AMyPlayerController::CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult)
{
	Atest1GameModeBase *gameMode = (Atest1GameModeBase*)GetWorld()->GetAuthGameMode();
	FVector flockCenter = gameMode->GetFlockCenter();

	FVector camLocation(0, -3000, 0);
	if (mFlockCamera)
		camLocation = mFlockCamera->GetActorLocation();

	FRotator camRot(0, 90, 0);	// 90 deg yaw around Z
	if (mFlockCamera)
		camRot = mFlockCamera->GetActorRotation();

	//
	// Rotate camera to look at flock center
	//
	FVector camDir = flockCenter - camLocation;
	camDir.Normalize();
	camRot = camDir.Rotation();

	OutResult.Location = camLocation;
	OutResult.Rotation = camRot;
	//	OutResult.FOV = 135.0f;

#if 0
	wchar_t tmp[64];
	swprintf(tmp, 64, L"MOOSE: camLocation %.2f, %.2f, %.2f", camLocation.X, camLocation.Y, camLocation.Z);
	UE_LOG(LogTemp, Warning, tmp);
#endif
}