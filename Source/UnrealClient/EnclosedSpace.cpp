#include "EnclosedSpace.h"
#include "BP_Fire.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AEnclosedSpace::AEnclosedSpace()
{
    PrimaryActorTick.bCanEverTick = true;

    // Static Mesh 컴포넌트 생성 및 기본 메시 설정
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // 기본 메시 설정 (예: 기본 큐브 메시)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        MeshComponent->SetStaticMesh(CubeMesh.Object);
    }

    CO_Concentration = 0.0f;
    SpaceVolume = 1000.0f; // 예시 값, 필요에 따라 조정
}

void AEnclosedSpace::BeginPlay()
{
    Super::BeginPlay();

    // FTimerDelegate를 사용하여 정확한 함수 호출
    FTimerDelegate TimerDel;
    TimerDel.BindUFunction(this, FName("UpdateCOConcentration"), 1.0f);

    // 타이머 설정
    GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 1.0f, true);
}

void AEnclosedSpace::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AEnclosedSpace::UpdateCOConcentration(float DeltaTime)
{
    TArray<AActor*> FoundFires;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABP_Fire::StaticClass(), FoundFires);

    for (AActor* FireActor : FoundFires)
    {
        ABP_Fire* Fire = Cast<ABP_Fire>(FireActor);
        if (Fire)
        {
            CO_Concentration += (Fire->CO_EmissionRate * DeltaTime) / SpaceVolume;
        }
    }

    // UE_LOG를 사용하여 일산화탄소 농도를 출력
    UE_LOG(LogTemp, Warning, TEXT("Current CO Concentration: %f"), CO_Concentration);
}
