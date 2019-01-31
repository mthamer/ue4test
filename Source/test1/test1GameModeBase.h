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

	const FVector &GetFlockCenter() { return mFlockCenter;  }
//	void SetFlockGoal(const FVector &goal) { mFlockGoal = goal; }

private:
	void MoveAllBoids(float DeltaTime);
	void CalcFlockCenter();
	void CalcFlockGoal();

	FVector RuleMoveTowardsFlockCenter(ABoidActor *b, float DeltaTime);
	FVector RuleAvoidBoids(ABoidActor *b, float DeltaTime);
	FVector RuleMatchVelocity(ABoidActor *b, float DeltaTime);
	FVector RuleMoveTowardsGoal(ABoidActor *b, float DeltaTime);

	UPROPERTY(EditAnywhere)
	float mSpeed;
	ABoidActor *mFlock[NUM_BOIDS];
	FVector mFlockCenter;
	FVector mFlockGoal;
};
