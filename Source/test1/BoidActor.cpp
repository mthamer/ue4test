// Fill out your copyright notice in the Description page of Project Settings.

#include "BoidActor.h"

#include "Engine/World.h"
#include "BoidActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "ConstructorHelpers.h"

// Sets default values
ABoidActor::ABoidActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//
	// CREATE MESH
	//

	UStaticMeshComponent* MyMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), TEXT("Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("MOOSE FOUND SPHERE"));
		MyMeshComponent->SetStaticMesh(SphereVisualAsset.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MOOSE DID NOT FIND SPHERE"));
	}

#if 0
	UStaticMeshComponent* MyMeshComponent = NewObject<UStaticMeshComponent>(obj, UStaticMeshComponent::StaticClass(), TEXT("Mesh"));
	UStaticMesh* MeshAsset = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Content/StarterContent/Shapes/Shape_Sphere'")));
	MyMeshComponent->SetStaticMesh(MeshAsset);
#endif

	//	UMaterial* MaterialAsset = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("Material'/Game/Weapons/axes/doubleaxe02c_Mat.doubleaxe02c_Mat'")));


	FVector newlocation(0, 0, 0);
	FRotator rotation = FRotator::ZeroRotator;

//	MyMeshComponent->SetMaterial(0, MaterialAsset);
	MyMeshComponent->SetWorldLocation(newlocation);
	MyMeshComponent->SetIsReplicated(true);

//	MyMeshComponent->RegisterComponent();
	AddOwnedComponent(MyMeshComponent);
	SetRootComponent(MyMeshComponent);

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

