#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnclosedSpace.generated.h"

UCLASS()
class UNREALCLIENT_API AEnclosedSpace : public AActor
{
    GENERATED_BODY()

public:
    AEnclosedSpace();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // ���� �� �ϻ�ȭź�� ��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CO")
    float CO_Concentration;

    // ������ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CO")
    float SpaceVolume;

    // Static Mesh ������Ʈ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

private:
    UFUNCTION()
    void UpdateCOConcentration(float DeltaTime);

    FTimerHandle TimerHandle;
};
