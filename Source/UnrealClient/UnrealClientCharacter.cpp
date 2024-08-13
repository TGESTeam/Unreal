// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealClientCharacter.h"
#include "UnrealClientProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AUnrealClientCharacter

AUnrealClientCharacter::AUnrealClientCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));


	NumX = 85;
	NumY = 93;
	NumZ = 101;

	FVector Dimensions = FVector(1633.0f, 1809.0f, 2014.0f);
	VoxelSize = FVector(Dimensions.X / NumX, Dimensions.Y / NumY, Dimensions.Z / NumZ);




}

void AUnrealClientCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	UE_LOG(LogTemplateCharacter, Log, TEXT("here1"));

	UE_LOG(LogTemplateCharacter, Log, TEXT("here5"));

	// 복셀 간의 간격을 복셀 크기에 맞게 설정
	float VoxelSpacingY = VoxelSize.Y;
	float VoxelSpacingZ = VoxelSize.Z;

	// 카메라의 위치와 방향 가져오기
	FVector CameraLocation = GetFirstPersonCameraComponent()->GetComponentLocation();
	FRotator CameraRotation = GetFirstPersonCameraComponent()->GetComponentRotation();
	FVector ForwardVector = CameraRotation.Vector();

	// 그리드의 중앙이 카메라 앞에 오도록 위치 설정
	FVector StartLocation = CameraLocation + ForwardVector * DistanceFromCamera;

	// 복셀 생성 및 배열에 저장
	for (int32 i = 0; i < GridSizeY; ++i)
	{
		for (int32 j = 0; j < GridSizeZ; ++j)
		{
			// Y와 Z 축에 대해 그리드 위치 계산
			FVector VoxelLocation = StartLocation + FVector(0.0f, i * VoxelSpacingY, j * VoxelSpacingZ)
				- FVector(0.0f, (GridSizeY - 1) * VoxelSpacingY / 2.0f, (GridSizeZ - 1) * VoxelSpacingZ / 2.0f);

			// Voxel 스폰 파라미터 설정
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// AVoxel_one 클래스를 직접 스폰
			AVoxel_one* SpawnedVoxel = GetWorld()->SpawnActor<AVoxel_one>(AVoxel_one::StaticClass(), VoxelLocation, FRotator::ZeroRotator, SpawnParams);

			if (SpawnedVoxel)
			{
				SpawnedVoxels.Add(SpawnedVoxel);  // 배열에 추가
			}
			else
			{
				UE_LOG(LogTemplateCharacter, Error, TEXT("Failed to spawn Voxel at location: %s"), *VoxelLocation.ToString());
			}
		}
	}

	//// 복셀 간의 간격을 복셀 크기에 맞게 설정
	//float VoxelSpacingY = VoxelSize.Y;
	//float VoxelSpacingZ = VoxelSize.Z;

	//// 카메라의 위치와 방향 가져오기
	//FVector CameraLocation = GetFirstPersonCameraComponent()->GetComponentLocation();
	//FRotator CameraRotation = GetFirstPersonCameraComponent()->GetComponentRotation();
	//FVector ForwardVector = CameraRotation.Vector();

	//// 그리드의 중앙이 카메라 앞에 오도록 위치 설정
	//FVector StartLocation = CameraLocation + ForwardVector * 500.0f;

	//// 복셀 생성 및 배열에 저장
	//for (int32 i = 0; i < GridSize; ++i)
	//{
	//	for (int32 j = 0; j < GridSize; ++j)
	//	{
	//		// Y와 Z 축에 대해 그리드 위치 계산
	//		FVector VoxelLocation = StartLocation + FVector(0.0f, i * VoxelSpacingY, j * VoxelSpacingZ)
	//			- FVector(0.0f, (GridSize - 1) * VoxelSpacingY / 2.0f, (GridSize - 1) * VoxelSpacingZ / 2.0f);

	//		// Voxel 스폰 파라미터 설정
	//		FActorSpawnParameters SpawnParams;
	//		SpawnParams.Owner = this;
	//		SpawnParams.Instigator = GetInstigator();

	//		// AVoxel_one 클래스를 직접 스폰
	//		AVoxel_one* SpawnedVoxel = GetWorld()->SpawnActor<AVoxel_one>(AVoxel_one::StaticClass(), VoxelLocation, CameraRotation, SpawnParams);

	//		if (SpawnedVoxel)
	//		{
	//			SpawnedVoxel->SetActorEnableCollision(false); // 충돌 비활성화
	//			SpawnedVoxel->SetActorTickEnabled(false); // 틱 비활성화
	//			SpawnedVoxels.Add(SpawnedVoxel);  // 배열에 추가
	//		}
	//		else
	//		{
	//			UE_LOG(LogTemplateCharacter, Error, TEXT("Failed to spawn Voxel at location: %s"), *VoxelLocation.ToString());
	//		}
	//	}
	//}


}

//////////////////////////////////////////////////////////////////////////// Input

void AUnrealClientCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUnrealClientCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUnrealClientCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AUnrealClientCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 복셀 간의 간격을 복셀 크기에 맞게 설정
	float VoxelSpacingY = VoxelSize.Y;
	float VoxelSpacingZ = VoxelSize.Z;

	// 카메라의 위치와 방향 가져오기
	FVector CameraLocation = GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector CameraForward = GetFirstPersonCameraComponent()->GetForwardVector();
	FVector CameraRight = GetFirstPersonCameraComponent()->GetRightVector();
	FVector CameraUp = GetFirstPersonCameraComponent()->GetUpVector();

	// 그리드의 중앙이 카메라 앞에 오도록 위치 설정
	FVector GridCenterLocation = CameraLocation + CameraForward * DistanceFromCamera;

	// 모든 복셀의 위치 업데이트
	for (int32 i = 0; i < GridSizeY; ++i)
	{
		for (int32 j = 0; j < GridSizeZ; ++j)
		{
			int32 Index = i * GridSizeZ + j;
			if (SpawnedVoxels.IsValidIndex(Index) && SpawnedVoxels[Index])
			{
				// Y와 Z 축에 대해 그리드 위치 계산
				FVector VoxelLocation = GridCenterLocation
					+ (CameraRight * (i - GridSizeY / 2) * VoxelSpacingY)
					+ (CameraUp * (j - GridSizeZ / 2) * VoxelSpacingZ);

				// 복셀 위치 및 회전 설정 (회전은 카메라를 바라보도록 설정)
				SpawnedVoxels[Index]->SetActorLocation(VoxelLocation);
				SpawnedVoxels[Index]->SetActorRotation(FRotationMatrix::MakeFromX(CameraForward).Rotator());
			}
		}
	}

	//// 복셀 간의 간격을 복셀 크기에 맞게 설정
	//float VoxelSpacingY = VoxelSize.Y;
	//float VoxelSpacingZ = VoxelSize.Z;

	//// 카메라의 위치와 방향 가져오기
	//FVector CameraLocation = GetFirstPersonCameraComponent()->GetComponentLocation();
	//FRotator CameraRotation = GetFirstPersonCameraComponent()->GetComponentRotation();
	//FVector ForwardVector = CameraRotation.Vector();

	//// 그리드의 중앙이 카메라 앞에 오도록 위치 설정
	//FVector StartLocation = CameraLocation + ForwardVector * 500.0f;

	//// 모든 복셀의 위치 및 회전 업데이트
	//for (int32 i = 0; i < GridSize; ++i)
	//{
	//	for (int32 j = 0; j < GridSize; ++j)
	//	{
	//		int32 Index = i * GridSize + j;
	//		if (SpawnedVoxels.IsValidIndex(Index) && SpawnedVoxels[Index])
	//		{
	//			// Y와 Z 축에 대해 그리드 위치 계산
	//			FVector VoxelLocation = StartLocation + FVector(0.0f, i * VoxelSpacingY, j * VoxelSpacingZ)
	//				- FVector(0.0f, (GridSize - 1) * VoxelSpacingY / 2.0f, (GridSize - 1) * VoxelSpacingZ / 2.0f);
	//			SpawnedVoxels[Index]->SetActorLocation(VoxelLocation);
	//			SpawnedVoxels[Index]->SetActorRotation(CameraRotation);  // 카메라의 회전 방향에 맞추기
	//		}
	//	}
	//}

}


void AUnrealClientCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AUnrealClientCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}