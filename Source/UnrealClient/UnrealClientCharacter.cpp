#include "UnrealClientCharacter.h"
#include "UnrealClientProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "ProtocolLibrary.h"
#include <Kismet/GameplayStatics.h>
#include "Voxel_one.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AUnrealClientCharacter::AUnrealClientCharacter() 
{
	UE_LOG(LogTemp, Warning, TEXT("HelloHello"));
	//Voxel 그리드 크기와 간격 설정
	
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
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));




}

void AUnrealClientCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//VoxelGridSize = 20;
	////VoxelSpacing = 100.0f; // Voxel 간의 거리
	//// ProtocolLibrary 싱글톤 인스턴스 가져오기
	//ProtocolLibraryInstance = AProtocolLibrary::GetInstance(GetWorld());
	//if (!ProtocolLibraryInstance)
	//{
	//	UE_LOG(LogTemplateCharacter, Error, TEXT("Failed to get ProtocolLibrary instance."));

	//}



	// Voxel 그리드를 생성합니다.
	//CreateVoxelGrid();


	// CO2 데이터를 생성하고 적용합니다.
	//GenerateCO2Data();



	/*
	// --------- CPP <-- Python으로 데이터 받을 때 SL 해석 테스트 [Start] -----------
	// 1. 사용자와 연결(가정)

	// 2. receive를 처음으로 받는다. (while문으로 앞으로 여러번 받을 예정)
	 //   -> 문자열 형태:"SL50000SH10,50,30,5PV1.333,2.333,3.333,4.4444,5.5555,6.6666"
	uint8 RawData[] = { // 예시용 데이터
		83, 76, 53, 48, 48, 48, 48, 83, 72, 49, 48, 44, 53, 48, 44, 51, 48, 44, 53, 80, 86,
		49, 46, 51, 51, 51, 44, 50, 46, 51, 51, 51, 44, 51, 46, 51, 51, 51, 44, 52, 46, 52,
		52, 52, 52, 44, 53, 46, 53, 53, 53, 53, 44, 54, 46, 54, 54, 54, 54
	};

	FString ParsedData;
	TArray<uint8> ReceivedData; // 예시 데이터를 넣을 Array
	TArray<uint8> ReceivedData2; // 예시 데이터를 넣을 Array

	// RawData 배열에서 데이터를 TArray에 복사
	ReceivedData.Append(RawData, sizeof(RawData) / sizeof(uint8));

	// 3. 처음 받은 receive를 ParsingSL(VL 일부를 받을 FString)로 해석
	ProtocolLibraryInstance -> ParsingSL(ParsedData, ReceivedData);


	// 4. 모든 문자열이 담길 str이 공간이 할당되고, TArray에 있는 데이터가 들어가짐

	// 5. while문으로 다시 데이터를 받는다.
	//	  -> 문자열 형태:"5.555,6.666,7.777,8.888,9.999,10.101,11.111,12.121,13.131,14.141,15.151"
	uint8 RawData2[] = {53, 46, 53, 53, 53, 44, 54, 46, 54, 54, 54, 44, 55, 46, 55, 55,
		55, 44, 56, 46, 56, 56, 56, 44, 57, 46, 57, 57, 57, 44, 49, 48, 46, 49, 48, 49,
		44, 49, 49, 46, 49, 49, 49, 44, 49, 50, 46, 49, 50, 49, 44, 49, 51, 46, 49, 51,
		49, 44, 49, 52, 46, 49, 52, 49, 44, 49, 53, 46, 49, 53, 49
	};
	// RawData 배열에서 데이터를 TArray에 복사
	ReceivedData2.Append(RawData2, sizeof(RawData2) / sizeof(uint8));

	// 6. 데이터_담기(모든 문자열이 담길 str, 현재 받은 문자열을 담은 TArray)
	ProtocolLibraryInstance->ParsingReceiveData(ParsedData, ReceivedData2);

	 //--------- CPP <-- Python으로 데이터 받을 때 SL 해석 테스트 [End] -----------
	 */
}

void AUnrealClientCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// 게임 종료 시 싱글톤 인스턴스 해제
	AProtocolLibrary::DestroyInstance();
}

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

	// 플레이어가 움직일 때마다 복셀을 업데이트
	//UpdateVoxels();
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

void AUnrealClientCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (ProtocolLibraryInstance)
	//{
	//	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	//	if (PlayerCharacter)
	//	{
	//		//Player Location
	//		FVector PlayerLocation = PlayerCharacter->GetActorLocation();

	//		//ProtocolLibrary singleton instance LOVector change Value
	//		ProtocolLibraryInstance->PlayerLocation.X = PlayerLocation.X;
	//		ProtocolLibraryInstance->PlayerLocation.Y = PlayerLocation.Y;
	//		ProtocolLibraryInstance->PlayerLocation.Z = PlayerLocation.Z;

	//		//PlayerViewdirection
	//		FRotator PlayerDirection = PlayerCharacter->GetActorRotation();
	//		//ProtocolLibrary singleton instance ROVector change Value
	//		ProtocolLibraryInstance->PlayerViewDirection.Pitch = PlayerDirection.Pitch;
	//		ProtocolLibraryInstance->PlayerViewDirection.Yaw = PlayerDirection.Yaw;
	//		ProtocolLibraryInstance->PlayerViewDirection.Roll = PlayerDirection.Roll;

	//		/*UE_LOG(LogTemp, Log, TEXT("Player Location Updated: %lf, %lf, %lf"), PlayerLocation.X, PlayerLocation.Y, PlayerLocation.Z);*/
	//	}
	//}
	//else
	//{
	//	UE_LOG(LogTemplateCharacter, Warning, TEXT("ProtocolLibraryInstance is null."));
	//}



	// Voxel 그리드의 위치를 업데이트합니다.
	//UpdateVoxelGrid();
}



//void AUnrealClientCharacter::CreateVoxelGrid()
//{
//	FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
//	FVector Origin = FirstPersonCameraComponent->GetComponentLocation() + ForwardVector * 500.0f; // 캐릭터 앞 500 단위 위치
//
//	// 그리드의 시작점을 계산
//	FVector GridStart = Origin - FVector(VoxelGridSize / 2 * VoxelSpacing, VoxelGridSize / 2 * VoxelSpacing, 0);
//
//	for (int32 x = 0; x < VoxelGridSize; x++)
//	{
//		for (int32 y = 0; y < VoxelGridSize; y++)
//		{
//			// Voxel 위치 계산
//			FVector VoxelPosition = GridStart + FVector(x * VoxelSpacing, y * VoxelSpacing, 0);
//
//			// Voxel_one 액터를 생성합니다.
//			AVoxel_one* NewVoxel = GetWorld()->SpawnActor<AVoxel_one>(VoxelPosition, FRotator::ZeroRotator);
//			if (NewVoxel)
//			{
//				VoxelGrid.Add(NewVoxel);
//			}
//		}
//	}
//}
//
//
//void AUnrealClientCharacter::UpdateVoxelGrid()
//{
//	FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
//	FVector Origin = FirstPersonCameraComponent->GetComponentLocation() + ForwardVector * 500.0f; // 캐릭터 앞 500 단위 위치
//
//	// 그리드의 시작점을 계산
//	FVector GridStart = Origin - FVector(VoxelGridSize / 2 * VoxelSpacing, VoxelGridSize / 2 * VoxelSpacing, 0);
//
//	// Voxel 위치 업데이트
//	for (int32 x = 0; x < VoxelGridSize; x++)
//	{
//		for (int32 y = 0; y < VoxelGridSize; y++)
//		{
//			int32 Index = x * VoxelGridSize + y;
//			if (VoxelGrid.IsValidIndex(Index))
//			{
//				FVector VoxelPosition = GridStart + FVector(x * VoxelSpacing, y * VoxelSpacing, 0);
//				VoxelGrid[Index]->SetActorLocation(VoxelPosition);
//			}
//		}
//	}
//}
//
//void AUnrealClientCharacter::ApplyCO2Data(const TArray<float>& CO2Values)
//{
//	int32 Index = 0;
//	for (AVoxel_one* Voxel : VoxelGrid)
//	{
//		if (Voxel && CO2Values.IsValidIndex(Index))
//		{
//			// Voxel의 머티리얼에 이산화탄소 값을 적용합니다.
//			Voxel->SetCO2Value(CO2Values[Index]);
//			Index++;
//		}
//	}
//}
//
//
//void AUnrealClientCharacter::GenerateCO2Data()
//{
//	TArray<float> CO2Values;
//
//	// Voxel의 개수만큼 CO2 값을 생성
//	for (int32 i = 0; i < VoxelGridSize * VoxelGridSize; i++)
//	{
//		// 0.0f에서 1.0f 사이의 랜덤 값을 생성하여 CO2Values 배열에 추가
//		float RandomCO2Value = FMath::FRand(); // 0.0f에서 1.0f 사이의 랜덤 값
//		CO2Values.Add(RandomCO2Value);
//	}
//
//	// 생성된 CO2 데이터를 ApplyCO2Data 함수로 전달하여 적용
//	ApplyCO2Data(CO2Values);
//}




