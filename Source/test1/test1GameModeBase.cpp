// Fill out your copyright notice in the Description page of Project Settings.

#include "test1GameModeBase.h"

#include "Engine/World.h"
#include "BoidActor.h"

void Atest1GameModeBase::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("MOOSE START PLAY"));
	
	Super::BeginPlay();

	FVector newlocation(0, 0, 0);
	FRotator rotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = Instigator;
	ABoidActor* obj = GetWorld()->SpawnActor<ABoidActor>(ABoidActor::StaticClass(), newlocation, rotation, SpawnInfo);

}




