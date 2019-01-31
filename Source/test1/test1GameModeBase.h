// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "test1GameModeBase.generated.h"

#define NUM_BOIDS 50

/**
 * 
 */
class ABoidActor;
UCLASS()
class TEST1_API Atest1GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

		virtual void BeginPlay() override;

public:
	Atest1GameModeBase();
	virtual void Tick(float DeltaTime) override;
	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

	const FVector &GetFlockCenter() { return mFlockCenter;  }

private:
	void MoveAllBoids(float DeltaTime);
	void CalcFlockCenter();

	FVector Rule1(ABoidActor *b, float DeltaTime);
	FVector Rule2(ABoidActor *b, float DeltaTime);
	FVector Rule3(ABoidActor *b, float DeltaTime);
	ABoidActor *mFlock[NUM_BOIDS];
	FVector mFlockCenter;
};
