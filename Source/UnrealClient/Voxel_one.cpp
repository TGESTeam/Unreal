// Fill out your copyright notice in the Description page of Project Settings.


#include "Voxel_one.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"



// Sets default values
AVoxel_one::AVoxel_one()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;


    int32 NumX = 85;
    int32 NumY = 93;
    int32 NumZ = 101;

    // Set default voxel size
    //VoxelSize = FVector(14.58125f, 14.9976f, 15.2614f);
    VoxelSize = FVector(Dimensions.X / NumX, Dimensions.Y / NumY, Dimensions.Z / NumZ);

    // Create mesh component
    VoxelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VoxelMesh"));
    RootComponent = VoxelMesh;

    // Set the default mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> VoxelMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (VoxelMeshAsset.Succeeded())
    {
        VoxelMesh->SetStaticMesh(VoxelMeshAsset.Object);
    }




    // Create a dynamic material instance
    VoxelMaterial = UMaterialInstanceDynamic::Create(VoxelMesh->GetMaterial(0), this);
    if (VoxelMaterial)
    {
        VoxelMesh->SetMaterial(0, VoxelMaterial);
    }


    

    // Calculate scale based on the desired size in cm
    FVector Scale = VoxelSize / 100.0f;
    VoxelMesh->SetWorldScale3D(Scale);
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


// Set the CO2 value and update the material
void AVoxel_one::SetCO2Value(float CO2Value)
{
    if (VoxelMaterial)
    {
        // Assuming you have a material parameter called "CO2Value" to control the color or some other visual property
        FLinearColor VoxelColor = FLinearColor::LerpUsingHSV(FLinearColor::Blue, FLinearColor::Red, CO2Value);
        VoxelMaterial->SetScalarParameterValue(TEXT("CO2Value"), CO2Value);
    }
}

