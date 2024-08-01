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

    // 공간 내 일산화탄소 농도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CO")
    float CO_Concentration;

    // 공간의 부피
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CO")
    float SpaceVolume;

    // Static Mesh 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

private:
    UFUNCTION()
    void UpdateCOConcentration(float DeltaTime);

    FTimerHandle TimerHandle;
};
