// Fill out your copyright notice in the Description page of Project Settings.

/*
	The primary class that specifies which classes to use(PlayerController, Pawn, HUD, GameState, PlayerState)
	and commonly used to specify game rules for modes such as ‘Capture the Flag’ where it could handle the flags,
	or to handle ‘wave spawns’ in a wave based shooter.Handles other key features like spawning the player as well.

	Unreal Engine uses a left-handed, z-up world coordinate system
*/

//
// TODO:
// - add bird model with rotation and animation
// - Bounding the position
//

#include "test1GameModeBase.h"

#include "Engine/World.h"
#include "BoidActor.h"
#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"

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
	mFlockGoal = FVector::ZeroVector;

	// default values
	Speed = 4.f;
	FlockCenterWeight = 2.f;
	AvoidBoidsWeight = 3.f;
	MatchVelocityWeight = 1.f;
	MoveTowardsGoalWeight = 3.f;
	MaxVelMagnitude = 80.0f;
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
		FVector newlocation((rand() % maxDim * 2) - maxDim, (rand() % maxDim * 2) - maxDim, (rand() % maxDim * 2) - maxDim);
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
	CalcFlockGoal();
}

void wrap(float &f)
{
	if (f > 1000)
		f = -1000;
	else
		if (f < -1000)
			f = 1000;
}

//
// cap the velocity
//
void Atest1GameModeBase::LimitVelocity(FVector &vel, float limit)
{
	if (vel.Size() > limit)
	{
		vel.Normalize();
		vel = vel * limit;
	}
}

void Atest1GameModeBase::MoveAllBoids(float DeltaTime)
{
	FVector v1 = FVector::ZeroVector;
	FVector v2 = FVector::ZeroVector;
	FVector v3 = FVector::ZeroVector;
	FVector v4 = FVector::ZeroVector;
	for (ABoidActor *b : mFlock)
	{
		v1 = RuleMoveTowardsFlockCenter(b, DeltaTime);
		v2 = RuleAvoidBoids(b, DeltaTime);
		v3 = RuleMatchVelocity(b, DeltaTime);
		v4 = RuleMoveTowardsGoal(b, DeltaTime);

		FVector vel = b->GetBoidVelocity();
		vel = vel + v1 * FlockCenterWeight + v2 * AvoidBoidsWeight + v3 * MatchVelocityWeight + v4 * MoveTowardsGoalWeight;
		LimitVelocity(vel, MaxVelMagnitude);
		b->SetBoidVelocity(vel);

		FVector newPos = b->GetActorLocation() + vel * DeltaTime * Speed;

#if 0
		// wrap coords
		wrap(newPos.X);
		wrap(newPos.Y);
		wrap(newPos.Z);
#endif

		b->SetActorLocation(newPos);
	}
}

// Rule: Boids try to fly towards the centre of mass of neighbouring boids.
FVector Atest1GameModeBase::RuleMoveTowardsFlockCenter(ABoidActor *bIn, float DeltaTime)
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

// Rule: Boids try to keep a small distance away from other boids.
FVector Atest1GameModeBase::RuleAvoidBoids(ABoidActor *bIn, float DeltaTime)
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

//Rule: Boids try to match velocity with other boids.
FVector Atest1GameModeBase::RuleMatchVelocity(ABoidActor *bIn, float DeltaTime)
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

// Rule 1: Boids try to fly towards the centre of mass of neighbouring boids.
FVector Atest1GameModeBase::RuleMoveTowardsGoal(ABoidActor *bIn, float DeltaTime)
{
	FVector dir = (mFlockGoal - bIn->GetActorLocation());
	dir.Normalize();
	return dir;
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

// set the flock goal to the location of DefaultPawn
void Atest1GameModeBase::CalcFlockGoal()
{
	// search for FlockCamera actor, TODO - expose this in editor
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), FoundActors);

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		if (FoundActors[i]->GetName().Contains("DefaultPawn"))
		{
			mFlockGoal = FoundActors[i]->GetActorLocation();
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("MOOSE: Couldn't find DefaultPawn"));
}