// Fill out your copyright notice in the Description page of Project Settings.

/*
	The primary class that specifies which classes to use(PlayerController, Pawn, HUD, GameState, PlayerState)
	and commonly used to specify game rules for modes such as ‘Capture the Flag’ where it could handle the flags,
	or to handle ‘wave spawns’ in a wave based shooter.Handles other key features like spawning the player as well.
*/

//
// TODO:
// - expose flock params to editor
// - create dynamic camera which follows flock from a dist
// - add bird model with rotation and animation
//

#include "test1GameModeBase.h"

#include "Engine/World.h"
#include "BoidActor.h"
#include "MyPlayerController.h"

Atest1GameModeBase::Atest1GameModeBase()
{
	// enable ticking
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	//tell your custom game mode to use your custom player controller
	PlayerControllerClass = AMyPlayerController::StaticClass();

#if 0
	//you can set whatever (if any) other default framework classes
	//you wish for this game mode as well
	DefaultPawnClass = ACustomPawn::StaticClass();
	GameStateClass = ACustomGameState::StaticClass();
	HUDClass = ACustomGameHUD::StaticClass();
	ReplaySpectatorPlayerControllerClass = ACustomReplaySpectatorPlayerController::StaticClass();
	SpectatorClass = ACustomSpectatorClass::StaticClass();
#endif

	mFlockCenter = FVector::ZeroVector;
}

void Atest1GameModeBase::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("MOOSE: BEGIN PLAY"));

	Super::BeginPlay();

	FRotator rotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = Instigator;

	const int maxDim = 500;
	for (int i = 0; i < NUM_BOIDS; i++)
	{
		FVector newlocation(0, (rand() % maxDim * 2) - maxDim, (rand() % maxDim * 2) - maxDim);
		mFlock[i] = GetWorld()->SpawnActor<ABoidActor>(ABoidActor::StaticClass(), newlocation, rotation, SpawnInfo);
	}
	CalcFlockCenter();
}

void Atest1GameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// update the position and velocity of each boid
	MoveAllBoids(DeltaTime);
	CalcFlockCenter();
}

void wrap(float &f)
{
	if (f > 1000)
		f = -1000;
	else
		if (f < -1000)
			f = 1000;
}

void Atest1GameModeBase::MoveAllBoids(float DeltaTime)
{
	const float speed = 5.f;
	FVector v1 = FVector::ZeroVector;
	FVector v2 = FVector::ZeroVector;
	FVector v3 = FVector::ZeroVector;
	for (ABoidActor *b : mFlock)
	{
		v1 = Rule1(b, DeltaTime);
		v2 = Rule2(b, DeltaTime);
		v3 = Rule3(b, DeltaTime);

		FVector vel = b->GetBoidVelocity();
		vel = vel + v1 + v2 + v3;
		b->SetBoidVelocity(vel);

		FVector newPos = b->GetActorLocation() + vel * DeltaTime * speed;

#if 0
		// wrap coords
		wrap(newPos.X);
		wrap(newPos.Y);
		wrap(newPos.Z);
#endif

		b->SetActorLocation(newPos);
	}
}

// Rule 1: Boids try to fly towards the centre of mass of neighbouring boids.
FVector Atest1GameModeBase::Rule1(ABoidActor *bIn, float DeltaTime)
{
	FVector pos = FVector::ZeroVector;
	for (ABoidActor *b : mFlock)
	{
		if (b != bIn)
		{
			pos = pos + b->GetActorLocation();
		}
	}

	pos = pos / (NUM_BOIDS - 1);
	FVector dir = (pos - bIn->GetActorLocation());
	dir.Normalize();
	return dir;
}

// Rule 2: Boids try to keep a small distance away from other objects(including other boids).
FVector Atest1GameModeBase::Rule2(ABoidActor *bIn, float DeltaTime)
{
	int closeCnt = 0;
	const float nearDist = 100.0f;		// sphere diameter is 10
	FVector pos = FVector::ZeroVector;
	for (ABoidActor *b : mFlock)
	{
		if (b != bIn)
		{
			FVector vecTo = b->GetActorLocation() - bIn->GetActorLocation();
			if (vecTo.Size() < nearDist)
			{
				pos = pos - vecTo;
				closeCnt++;
			}
		}
	}

	if (closeCnt > 0)
	{
		wchar_t tmp[64];
		swprintf(tmp, 64, L"MOOSE: found %d close boids", closeCnt);
		UE_LOG(LogTemp, Warning, tmp);
	}

	pos.Normalize();
	return pos;
}

//Rule 3: Boids try to match velocity with other boids.
FVector Atest1GameModeBase::Rule3(ABoidActor *bIn, float DeltaTime)
{
	FVector vel = FVector::ZeroVector;
	for (ABoidActor *b : mFlock)
	{
		if (b != bIn)
		{
			vel = vel + b->GetBoidVelocity();
		}
	}

	//	vel = vel / (NUM_BOIDS - 1);
	//	FVector dir = (vel - bIn->GetBoidVelocity());
	FVector dir = vel;
	dir.Normalize();
	return dir;
}

void Atest1GameModeBase::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	UE_LOG(LogTemp, Warning, TEXT("MOOSE: CalcCamera"));

}

void Atest1GameModeBase::CalcFlockCenter()
{
	FVector pos = FVector::ZeroVector;
	for (ABoidActor *b : mFlock)
	{
		pos = pos + b->GetActorLocation();
	}
	mFlockCenter = pos / NUM_BOIDS;
}