// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelManager.h"
#include "Engine/World.h"

// Sets default values
AVoxelManager::AVoxelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    UE_LOG(LogTemp, Warning, TEXT("VoxelManager Constructor"));
}

// Called when the game starts or when spawned
void AVoxelManager::BeginPlay()
{
	Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("VoxelManager BeginPlay"));
	GenerateVoxels();
	
}

// Called every frame
void AVoxelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    //UE_LOG(LogTemp, Warning, TEXT("VoxelManager Tick"));

}


void AVoxelManager::GenerateVoxels()
{
    if (VoxelClass)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            for (int32 X = 0; X < GridSize; ++X)
            {
                for (int32 Y = 0; Y < GridSize; ++Y)
                {
                    for (int32 Z = 0; Z < GridSize; ++Z)
                    {
                        FVector Location(X * VoxelSpacing, Y * VoxelSpacing, Z * VoxelSpacing);
                        FActorSpawnParameters SpawnParams;
                        AVoxel_one* Voxel = World->SpawnActor<AVoxel_one>(VoxelClass, Location, FRotator::ZeroRotator, SpawnParams);
                        if (Voxel)
                        {
                            UE_LOG(LogTemp, Warning, TEXT("Spawned Voxel at (%f, %f, %f)"), Location.X, Location.Y, Location.Z);
                        }
                        else
                        {
                            UE_LOG(LogTemp, Error, TEXT("Failed to spawn Voxel at (%f, %f, %f)"), Location.X, Location.Y, Location.Z);
                        }
                    }
                }
            }
        }
    }
}
