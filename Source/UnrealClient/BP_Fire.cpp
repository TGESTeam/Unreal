#include "BP_Fire.h"
#include "Particles/ParticleSystemComponent.h"
#include "UObject/ConstructorHelpers.h"

ABP_Fire::ABP_Fire()
{
    PrimaryActorTick.bCanEverTick = true;

    // ��ƼŬ �ý��� ������Ʈ ���� �� �⺻ ��ƼŬ ����
    FireParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticleSystem"));
    RootComponent = FireParticleSystem;

    // �⺻ ��ƼŬ �ý��� ���� (��: �⺻ �� ��ƼŬ �ý���)
    static ConstructorHelpers::FObjectFinder<UParticleSystem> FireParticle(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
    if (FireParticle.Succeeded())
    {
        FireParticleSystem->SetTemplate(FireParticle.Object);
    }

    CO_EmissionRate = 0.1f; // ���� ��, �ʿ信 ���� ����
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

    // Static Mesh ������Ʈ ���� �� �⺻ �޽� ����
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // �⺻ �޽� ���� (��: �⺻ ť�� �޽�)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        MeshComponent->SetStaticMesh(CubeMesh.Object);
    }

    CO_EmissionRate = 0.1f; // ���� ��, �ʿ信 ���� ����
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