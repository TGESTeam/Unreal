// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BP_Fire.generated.h"

UCLASS()
class UNREALCLIENT_API ABP_Fire : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABP_Fire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// �ϻ�ȭź�� �߻��� (�ʴ� ������)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CO")
	float CO_EmissionRate;

	// ��ƼŬ �ý��� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UParticleSystemComponent* FireParticleSystem;
};



/*
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BP_Fire.generated.h"

UCLASS()
class UNREALCLIENT_API ABP_Fire : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABP_Fire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// �ϻ�ȭź�� �߻��� (�ʴ� ������)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CO")
	float CO_EmissionRate;

	// Static Mesh ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;
};*/