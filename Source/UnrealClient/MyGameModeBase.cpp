// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "Voxel_one.h"
#include "Engine/World.h"
#include "EngineUtils.h"

void AMyGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("startGameMode start"));


    UWorld* World = GetWorld();
    if (World)
    {
        // 월드 이름을 로그로 출력
        UE_LOG(LogTemp, Warning, TEXT("Current World: %s"), *World->GetName());
    }

    FillBuildingWithVoxels();
}

void AMyGameModeBase::FillBuildingWithVoxels()
{
    // apart_blender_fds_COO_CL 액터에 대한 참조를 가지고 있다고 가정
    AActor* Building = GetBuildingActor();
    if (Building == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Building actor not found!"));
        return;
    }

    FVector BuildingLocation = Building->GetActorLocation();
    FVector BuildingExtent = Building->GetComponentsBoundingBox().GetExtent();

    FVector VoxelSize = FVector(14.58125f, 14.9976f, 15.2614f);
    FVector VoxelHalfSize = VoxelSize / 2.0f;

    // BuildingExtent의 중심에 Voxel_one을 배치
    FVector VoxelLocation = BuildingLocation - BuildingExtent + BuildingExtent / 2.0f;

    // Voxel_one을 스폰
    GetWorld()->SpawnActor<AVoxel_one>(AVoxel_one::StaticClass(), VoxelLocation, FRotator::ZeroRotator);
}


//void AMyGameModeBase::FillBuildingWithVoxels()
//{
//    // apart_blender_fds_COO_CL 액터에 대한 참조를 가지고 있다고 가정
//    AActor* Building = GetBuildingActor();
//    if (Building == nullptr)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("Building actor not found!"));
//        return;
//    }
//    UE_LOG(LogTemp, Warning, TEXT("Building actor not found!"));
//    FVector BuildingLocation = Building->GetActorLocation();
//    FVector BuildingExtent = Building->GetComponentsBoundingBox().GetExtent();
//
//    FVector VoxelSize = FVector(14.58125f, 14.9976f, 15.2614f);
//    FVector VoxelHalfSize = VoxelSize / 2.0f;
//
//    int32 NumVoxelsX = FMath::CeilToInt(BuildingExtent.X / VoxelSize.X);
//    int32 NumVoxelsY = FMath::CeilToInt(BuildingExtent.Y / VoxelSize.Y);
//    int32 NumVoxelsZ = FMath::CeilToInt(BuildingExtent.Z / VoxelSize.Z);
//
//    UE_LOG(LogTemp, Warning, TEXT("location : %d, %d, %d!"), NumVoxelsX, NumVoxelsY, NumVoxelsZ);
//
//    //for (int32 x = 0; x < NumVoxelsX; ++x)
//    //{
//    //    for (int32 y = 0; y < NumVoxelsY; ++y)
//    //    {
//    //        for (int32 z = 0; z < NumVoxelsZ; ++z)
//    //        {
//    //            FVector VoxelLocation = BuildingLocation - BuildingExtent +
//    //                FVector(x * VoxelSize.X, y * VoxelSize.Y, z * VoxelSize.Z) +
//    //                VoxelHalfSize;
//
//    //            GetWorld()->SpawnActor<AVoxel_one>(AVoxel_one::StaticClass(), VoxelLocation, FRotator::ZeroRotator);
//    //        }
//    //    }
//    //}
//}

AActor* AMyGameModeBase::GetBuildingActor()
{
    // apart_blender_fds_COO_CL 액터를 찾고 반환하는 로직
    for (TActorIterator<AActor> It(GetWorld()); It; ++It)
    {
        //// Object Name 출력
        //UE_LOG(LogTemp, Warning, TEXT("Object Name: %s"), *It->GetName());

        //// Display Name 출력
        //UE_LOG(LogTemp, Warning, TEXT("Display Name: %s"), *It->GetActorLabel());

        //UE_LOG(LogTemp, Warning, TEXT("Found Actor: %s"), *It->GetName());
        //if (It->GetName().Contains(TEXT("apart_blender_fds_COO_CL")))
        //{
        //    UE_LOG(LogTemp, Warning, TEXT("startGameMode check"));
        //    return *It;
        //}
        if (It->GetActorLabel().Contains(TEXT("apart_blender_fds_COO_CL")))
        {
            //UE_LOG(LogTemp, Warning, TEXT("startGameMode check"));
            return *It;
        }
    }
    //UE_LOG(LogTemp, Warning, TEXT("startGameMode fail fail"));
    return nullptr;
}
