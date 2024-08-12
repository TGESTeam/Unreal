// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelManager.h"
#include "Voxel_one.h"
#include "Engine/World.h"
#include "CoreMinimal.h"

// Sets default values
AVoxelManager::AVoxelManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVoxelManager::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("startVoxelManager"));
	CreateVoxels();
}

// Called every frame
void AVoxelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVoxelManager::CreateVoxels()
{
    if (VoxelClass == nullptr) return;


    //FVector VoxelSize = FVector(14.58125f, 14.9976f, 15.2614f);
    //FVector VoxelScale = VoxelSize / 100.0f;
    
    /*
    int32 NumX = FMath::FloorToInt(Dimensions.X / VoxelSize.X);
    int32 NumY = FMath::FloorToInt(Dimensions.Y / VoxelSize.Y);
    int32 NumZ = FMath::FloorToInt(Dimensions.Z / VoxelSize.Z);*/
    int32 NumX = 85;
    int32 NumY = 93;
    int32 NumZ = 101;

    FVector VoxelSize = FVector(Dimensions.X/NumX, Dimensions.Y / NumY, Dimensions.Z / NumZ);
    FVector VoxelScale = VoxelSize / 100.0f;

    for (int32 X = 0; X < 40; X++)
    {
        for (int32 Y = 0; Y < 40; Y++)
        {
            for (int32 Z = 0; Z < 30; Z++)
            {
                FVector Location = Origin + FVector(X * VoxelSize.X, Y * VoxelSize.Y, Z * VoxelSize.Z);

//            // 고유한 이름을 생성
//            FString VoxelName = FString::Printf(TEXT("Voxel_%d_%d_%d"), X, Y, Z);

//            // 생성 파라미터에 이름을 설정
//            FActorSpawnParameters SpawnParams;
//            SpawnParams.Name = FName(*VoxelName);

//            // 복셀을 스폰하면서 고유한 이름을 설정
                AVoxel_one* NewVoxel = GetWorld()->SpawnActor<AVoxel_one>(VoxelClass, Location, FRotator::ZeroRotator);
            }

        }
    }


    //for (int32 X = 0; X < NumX; X++)
    //{
    //    for (int32 Y = 0; Y < NumY; Y++)
    //    {
    //        for (int32 Z = 10; Z < 21; Z++)
    //        {
    //            FVector Location = Origin + FVector(X * VoxelSize.X, Y * VoxelSize.Y, Z * VoxelSize.Z);

    //            // 고유한 이름을 생성
    //            FString VoxelName = FString::Printf(TEXT("Voxel_%d_%d_%d"), X, Y, Z);

    //            // 생성 파라미터에 이름을 설정
    //            FActorSpawnParameters SpawnParams;
    //            SpawnParams.Name = FName(*VoxelName);

    //            // 복셀을 스폰하면서 고유한 이름을 설정
    //            AVoxel_one* NewVoxel = GetWorld()->SpawnActor<AVoxel_one>(VoxelClass, Location, FRotator::ZeroRotator, SpawnParams);
    //        }
    //    }
    //}

}

//void AVoxelManager::CreateVoxels()
//{
//    if (VoxelClass == nullptr) return;
//
//    FVector VoxelSize = FVector(14.58125f, 14.9976f, 15.2614f);
//    FVector VoxelScale = VoxelSize / 100.0f;
//
//    int32 NumX = FMath::FloorToInt(Dimensions.X / VoxelSize.X);
//    int32 NumY = FMath::FloorToInt(Dimensions.Y / VoxelSize.Y);
//
//    float FixedZ = 0.0f;
//
//    for (int32 X = 0; X < NumX; X++)
//    {
//        for (int32 Y = 0; Y < NumY; Y++)
//        {
//            FVector Location = Origin + FVector(X * VoxelSize.X, Y * VoxelSize.Y, FixedZ);
//            GetWorld()->SpawnActor<AVoxel_one>(VoxelClass, Location, FRotator::ZeroRotator);
//        }
//    }
//}


void AVoxelManager::CreateVoxelsInEditor()
{
    UE_LOG(LogTemp, Warning, TEXT("CreateVoxelGrid function called"));
    CreateVoxels(); // 동일한 로직을 사용
}
