// Fill out your copyright notice in the Description page of Project Settings.

#include "test1GameModeBase.h"

#include "Engine/World.h"
#include "BoidActor.h"

Atest1GameModeBase::Atest1GameModeBase()
{
	// enable ticking
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void Atest1GameModeBase::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("MOOSE: BEGIN PLAY"));
	
	Super::BeginPlay();

	FRotator rotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = Instigator;

	float scaleFactor = 40.0f;
	float offset = 100.0f;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
//			FVector newlocation(0, j*scaleFactor-offset, i*scaleFactor+offset);
			FVector newlocation(0, (rand() % 1000) - 500, (rand() % 1000) - 500);
			mFlock[i*5+j] = GetWorld()->SpawnActor<ABoidActor>(ABoidActor::StaticClass(), newlocation, rotation, SpawnInfo);
		}
	}
}



void Atest1GameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// update the position and velocity of each boid
	MoveAllBoids(DeltaTime);
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
	const float speed = 20.f;
	FVector v1, v2, v3;
	for (ABoidActor *b : mFlock)
	{
		v1 = Rule1(b, DeltaTime);
		v2 = Rule2(b, DeltaTime);
		v3 = Rule3(b, DeltaTime);

		FVector vel = b->GetBoidVelocity();
		vel = vel + v1 + v2 + v3;
		vel = vel * DeltaTime * speed;
		b->SetBoidVelocity(vel);

		FVector pos = b->GetActorLocation();
		pos = pos + vel;

#if 0
		// wrap coords
		wrap(pos.X);
		wrap(pos.Y);
		wrap(pos.Z);
#endif

		b->SetActorLocation(pos);
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

	FVector dir = (pos - bIn->GetActorLocation()) / 50.0f;
	return dir;
}

// Rule 2: Boids try to keep a small distance away from other objects(including other boids).
FVector Atest1GameModeBase::Rule2(ABoidActor *bIn, float DeltaTime)
{
	int closeCnt = 0;
	const float nearDist = 100.0f;
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
	pos = pos / 25.f;
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

	vel = vel / (NUM_BOIDS - 1);

	FVector dir = (vel - bIn->GetBoidVelocity()) / 8.0f;
	return dir;
}
