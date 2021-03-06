// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidActor.generated.h"

UCLASS()
class TEST1_API ABoidActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetBoidVelocity() { return mVelocity;  }
	void SetBoidVelocity(const FVector vel) { mVelocity = vel; }

private:
	FVector mVelocity;
	bool AddSphereMesh();
};
