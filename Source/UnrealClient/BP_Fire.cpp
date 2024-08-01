#include "BP_Fire.h"
#include "Particles/ParticleSystemComponent.h"
#include "UObject/ConstructorHelpers.h"

ABP_Fire::ABP_Fire()
{
    PrimaryActorTick.bCanEverTick = true;

    // 파티클 시스템 컴포넌트 생성 및 기본 파티클 설정
    FireParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticleSystem"));
    RootComponent = FireParticleSystem;

    // 기본 파티클 시스템 설정 (예: 기본 불 파티클 시스템)
    static ConstructorHelpers::FObjectFinder<UParticleSystem> FireParticle(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
    if (FireParticle.Succeeded())
    {
        FireParticleSystem->SetTemplate(FireParticle.Object);
    }

    CO_EmissionRate = 0.1f; // 예시 값, 필요에 따라 조정
}

void ABP_Fire::BeginPlay()
{
    Super::BeginPlay();
}

void ABP_Fire::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}


/*
// BP_Fire.cpp
#include "BP_Fire.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ABP_Fire::ABP_Fire()
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

    CO_EmissionRate = 0.1f; // 예시 값, 필요에 따라 조정
}

void ABP_Fire::BeginPlay()
{
    Super::BeginPlay();
}

void ABP_Fire::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
*/