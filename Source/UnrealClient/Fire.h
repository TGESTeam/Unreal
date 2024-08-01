// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fire.generated.h"

UCLASS()
class UNREALCLIENT_API AFire : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 일산화탄소 발생률 (초당 생성량)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CO")
	float CO_EmissionRate;

};
