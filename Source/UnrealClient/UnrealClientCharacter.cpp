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

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

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
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void AUnrealClientCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// ProtocolLibrary 싱글톤 인스턴스 가져오기
	ProtocolLibraryInstance = AProtocolLibrary::GetInstance(GetWorld());
	if (!ProtocolLibraryInstance)
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("Failed to get ProtocolLibrary instance."));
	}
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

	if (ProtocolLibraryInstance)
	{
		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
		if (PlayerCharacter)
		{
			//Player Location
			FVector PlayerLocation = PlayerCharacter->GetActorLocation();

			//ProtocolLibrary singleton instance LOVector change Value
			ProtocolLibraryInstance->PlayerLocation.X = PlayerLocation.X;
			ProtocolLibraryInstance->PlayerLocation.Y = PlayerLocation.Y;
			ProtocolLibraryInstance->PlayerLocation.Z = PlayerLocation.Z;

			//PlayerViewdirection
			FRotator PlayerDirection = PlayerCharacter->GetActorRotation();
			//ProtocolLibrary singleton instance ROVector change Value
			ProtocolLibraryInstance->PlayerViewDirection.Pitch = PlayerDirection.Pitch;
			ProtocolLibraryInstance->PlayerViewDirection.Yaw = PlayerDirection.Yaw;
			ProtocolLibraryInstance->PlayerViewDirection.Roll = PlayerDirection.Roll;

			/*UE_LOG(LogTemp, Log, TEXT("Player Location Updated: %lf, %lf, %lf"), PlayerLocation.X, PlayerLocation.Y, PlayerLocation.Z);*/
		}
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Warning, TEXT("ProtocolLibraryInstance is null."));
	}
}
