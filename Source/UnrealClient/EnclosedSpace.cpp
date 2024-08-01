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

    // Static Mesh ������Ʈ ���� �� �⺻ �޽� ����
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // �⺻ �޽� ���� (��: �⺻ ť�� �޽�)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        MeshComponent->SetStaticMesh(CubeMesh.Object);
    }

    CO_Concentration = 0.0f;
    SpaceVolume = 1000.0f; // ���� ��, �ʿ信 ���� ����
}

void AEnclosedSpace::BeginPlay()
{
    Super::BeginPlay();

    // FTimerDelegate�� ����Ͽ� ��Ȯ�� �Լ� ȣ��
    FTimerDelegate TimerDel;
    TimerDel.BindUFunction(this, FName("UpdateCOConcentration"), 1.0f);

    // Ÿ�̸� ����
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

    // UE_LOG�� ����Ͽ� �ϻ�ȭź�� �󵵸� ���
    UE_LOG(LogTemp, Warning, TEXT("Current CO Concentration: %f"), CO_Concentration);
}
