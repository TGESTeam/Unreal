// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
#include "Voxel_one.generated.h"

UCLASS()
class UNREALCLIENT_API AVoxel_one : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVoxel_one();

	FVector Dimensions = FVector(1633.0f, 1809.0f, 2014.0f);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Voxel size
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel")
	FVector VoxelSize;

	// Mesh component
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* VoxelMesh;

	// CO2 value (for visualization)
	void SetCO2Value(float CO2Value);

	// Gameplay Tags
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer GameplayTags;

//private:
//	UMaterialInstanceDynamic* VoxelMaterial;
};
