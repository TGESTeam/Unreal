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

	virtual void Tick(float DeltaTime) override; // Tick �Լ� �������̵�

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

	////Voxel_one Voxel�� �����ϰ� ������ �� �ִ� ���� �߰�
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel")
	//TSubclassOf<AVoxel_one> VoxelClass;

private:
	// ������ Voxel�� �����ϴ� ������
	//AVoxel_one* SpawnedVoxel;
	TArray<AVoxel_one*> SpawnedVoxels;  // ������ �������� �����ϴ� �迭
	//int32 GridSize = 15;  // 15x15 �׸���
	int32 GridSizeY = 23;  // Y�� ������ �׸��� ũ��
	int32 GridSizeZ = 15;  // Z�� ������ �׸��� ũ��



	//float VoxelSpacing = 50.0f;  // ���� ���� ����
	float DistanceFromCamera = 200.0f; // ī�޶󿡼� �׸�������� �Ÿ�
	//FVector VoxelSize = FVector(100.0f, 100.0f, 100.0f);  // ���� ũ��
	FVector VoxelSize;
	int32 NumX;
	int32 NumY;
	int32 NumZ;

	// Set default voxel size
	//VoxelSize = FVector(14.58125f, 14.9976f, 15.2614f);

};

