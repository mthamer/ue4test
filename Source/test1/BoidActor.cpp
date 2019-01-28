// Fill out your copyright notice in the Description page of Project Settings.

#include "BoidActor.h"

#include "Engine/World.h"
#include "BoidActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ConstructorHelpers.h"

// STATIC
ConstructorHelpers::FObjectFinder<UStaticMesh> *SphereVisualAssetPtr = nullptr;
ConstructorHelpers::FObjectFinder<UMaterial> *TechMaterialPtr = nullptr;

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
		UE_LOG(LogTemp, Warning, TEXT("MOOSE: DID NOT FIND SPHERE"));
	}

	// find tech hex material 
	static ConstructorHelpers::FObjectFinder<UMaterial> TechMaterial(TEXT("/Game/StarterContent/Materials/M_Tech_Hex_Tile.M_Tech_Hex_Tile"));
	TechMaterialPtr = &TechMaterial;
	if (!TechMaterial.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("MOOSE: Did not find material"));
	}

	AddSphereMesh();

}

// Called when the game starts or when spawned
void ABoidActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABoidActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

	// set material
	UMaterial *techMaterial = TechMaterialPtr->Object;
	UMaterialInterface *techMatInst = UMaterialInstanceDynamic::Create(techMaterial, MyMeshComponent);
	MyMeshComponent->SetMaterial(0, techMatInst);

	FVector newlocation(0, 0, 0);
	FRotator rotation = FRotator::ZeroRotator;

	MyMeshComponent->SetWorldLocation(newlocation);
	MyMeshComponent->SetIsReplicated(true);
	AddOwnedComponent(MyMeshComponent);
	SetRootComponent(MyMeshComponent);

	return true;
}