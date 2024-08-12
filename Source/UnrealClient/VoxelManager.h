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

	UFUNCTION(BlueprintCallable, Category = "Voxel")
	void CreateVoxelsInEditor();


	UFUNCTION(BlueprintCallable, Category = "Voxel")
	void CreateVoxels();
private:


	UPROPERTY(EditAnywhere, Category = "Voxel Settings")
	TSubclassOf<class AVoxel_one> VoxelClass;

	UPROPERTY(EditAnywhere, Category = "Voxel Settings")
	FVector Origin = FVector(-4000.0f, 3000.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "Voxel Settings")
	FVector Dimensions = FVector(1633.0f, 1809.0f, 2014.0f);
};

