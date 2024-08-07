// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Voxel_one.h"
#include "VoxelManager.generated.h"

UCLASS()
class UNREALCLIENT_API AVoxelManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVoxelManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // Function to generate voxels
    void GenerateVoxels();

    // Voxel class to spawn
    UPROPERTY(EditAnywhere, Category = "Voxel")
    TSubclassOf<AVoxel_one> VoxelClass;

    // Number of voxels in each dimension
    UPROPERTY(EditAnywhere, Category = "Voxel")
    int32 GridSize = 100;

    // Distance between voxels
    UPROPERTY(EditAnywhere, Category = "Voxel")
    float VoxelSpacing = 1.0f;

};
