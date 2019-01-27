// Fill out your copyright notice in the Description page of Project Settings.

#include "BoidActor.h"

#include "Engine/World.h"
#include "BoidActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "ConstructorHelpers.h"

// STATIC
ConstructorHelpers::FObjectFinder<UStaticMesh> *SphereVisualAssetPtr = nullptr;

// Sets default values
ABoidActor::ABoidActor()
{
	mVelocity = FVector(((rand() % 20) - 10) / 10.f, ((rand() % 20) - 10)/10.f, ((rand() % 20) - 10) / 10.f);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// find sphere static mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	SphereVisualAssetPtr = &SphereVisualAsset;
	if (!SphereVisualAsset.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("MOOSE DID NOT FIND SPHERE"));
	}

	AddSphereMesh();

}

// Called when the game starts or when spawned
void ABoidActor::BeginPlay()
{
	Super::BeginPlay();
	
//	UE_LOG(LogTemp, Warning, TEXT("MOOSE: BoidActor BeginPlay"));

}

// Called every frame
void ABoidActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if 0
	FVector curPos = GetActorLocation();
	curPos.Z += .5f;
	SetActorLocation(curPos);
#endif
}

bool ABoidActor::AddSphereMesh()
{
	UE_LOG(LogTemp, Warning, TEXT("MOOSE: BoidActor AddSphereMesh"));

	UStaticMeshComponent* MyMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), TEXT("Mesh"));
	if (SphereVisualAssetPtr->Succeeded())
	{
		MyMeshComponent->SetStaticMesh(SphereVisualAssetPtr->Object);
		MyMeshComponent->SetWorldScale3D(FVector(0.1f));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MOOSE: DID NOT FIND SPHERE"));
	}

	FVector newlocation(0, 0, 0);
	FRotator rotation = FRotator::ZeroRotator;

	MyMeshComponent->SetWorldLocation(newlocation);
	MyMeshComponent->SetIsReplicated(true);
	AddOwnedComponent(MyMeshComponent);
	SetRootComponent(MyMeshComponent);

	return true;
}