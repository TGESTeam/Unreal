// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Voxel_one.generated.h"

UCLASS()
class UNREALCLIENT_API AVoxel_one : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVoxel_one();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Voxel size
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel")
	FVector VoxelSize = FVector(1.0f, 1.0f, 1.0f);

	// Mesh component
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* VoxelMesh;

};
