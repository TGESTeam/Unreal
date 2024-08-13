// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Voxel_one.h"
#include "UnrealClientCharacter.generated.h"



class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AUnrealClientCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
public:
	AUnrealClientCharacter();

protected:
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override; // Tick 함수 오버라이드

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	////Voxel_one Voxel을 생성하고 관리할 수 있는 변수 추가
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel")
	//TSubclassOf<AVoxel_one> VoxelClass;

private:
	// 스폰된 Voxel을 관리하는 포인터
	//AVoxel_one* SpawnedVoxel;
	TArray<AVoxel_one*> SpawnedVoxels;  // 생성된 복셀들을 저장하는 배열
	//int32 GridSize = 15;  // 15x15 그리드
	int32 GridSizeY = 23;  // Y축 방향의 그리드 크기
	int32 GridSizeZ = 15;  // Z축 방향의 그리드 크기



	//float VoxelSpacing = 50.0f;  // 복셀 간의 간격
	float DistanceFromCamera = 200.0f; // 카메라에서 그리드까지의 거리
	//FVector VoxelSize = FVector(100.0f, 100.0f, 100.0f);  // 복셀 크기
	FVector VoxelSize;
	int32 NumX;
	int32 NumY;
	int32 NumZ;

	// Set default voxel size
	//VoxelSize = FVector(14.58125f, 14.9976f, 15.2614f);

};

