// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "test1GameModeBase.h"

AMyPlayerController::AMyPlayerController()
{
	UE_LOG(LogTemp, Warning, TEXT("MOOSE: AMyPlayerController ctor"));

	// set these for CalcCamera to be called
	PrimaryActorTick.bCanEverTick = true;
	bAutoManageActiveCameraTarget = false;
}

void AMyPlayerController::CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult)
{
//	UE_LOG(LogTemp, Warning, TEXT("MOOSE: AMyPlayerController::CalcCamera"));

	Atest1GameModeBase *gameMode = (Atest1GameModeBase*)GetWorld()->GetAuthGameMode();
	FVector flockCenter = gameMode->GetFlockCenter();
	FVector camLocation = flockCenter;
	camLocation.X -= 750;	// desired cam location

	FVector direction = flockCenter - camLocation;
	direction.Normalize();
	FRotator camRot = FRotationMatrix::MakeFromX(direction).Rotator();

	OutResult.Location = camLocation;
	OutResult.Rotation = camRot;
	OutResult.FOV = 135.0f;

	wchar_t tmp[64];
	swprintf(tmp, 64, L"MOOSE: camLocation %.2f, %.2f, %.2f", camLocation.X, camLocation.Y, camLocation.Z);
	UE_LOG(LogTemp, Warning, tmp);

}