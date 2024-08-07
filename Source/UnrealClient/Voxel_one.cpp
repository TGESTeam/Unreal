// Fill out your copyright notice in the Description page of Project Settings.


#include "Voxel_one.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AVoxel_one::AVoxel_one()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create mesh component
    VoxelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VoxelMesh"));
    RootComponent = VoxelMesh;

    // Set the default mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> VoxelMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (VoxelMeshAsset.Succeeded())
    {
        VoxelMesh->SetStaticMesh(VoxelMeshAsset.Object);
    }

    // Set the scale to match the desired voxel size
    VoxelMesh->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
    //VoxelMesh->SetWorldScale3D(VoxelSize);
}

// Called when the game starts or when spawned
void AVoxel_one::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVoxel_one::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

