// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Networking.h"
#include "ProtocolLibrary.generated.h"


UCLASS()
class UNREALCLIENT_API AProtocolLibrary : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProtocolLibrary();
	~AProtocolLibrary();

	// Enum
	enum SeperatorEnum
	{ // 0~6
		LO,
		RO,
		VL,
		TI,
		SH,
		PV
	};

	//변수
	struct LOVector {
		double X;
		double Y;
		double Z;
	};

	struct ROVector {
		double Pitch;
		double Yaw;
		double Roll;
	};

	LOVector PlayerLocation;

	ROVector PlayerViewDirection;

	//SH size
	TArray<int32> SHArray;  // int32형 요소를 가지는 TArray 선언

	//now Time
	FDateTime CurrentTime;

	// PlayerLocation value print - 
	void PrintPlayerLocation() const;

	//PlayerViewDirection value print -
	void PrintPlayerViewDirection() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ConnectToServer(const FString& ServerAddress, int32 Port);
	void SendData(const FString& Data);
	FString ReceiveData();
	//void ParsingSL(FString& ParsedData, const TArray<uint8>& ReceivedData);
	void ParsingSL(FString& ParsedData);
	// SL길이만큼 str 할당 후, 이후의 데이터 해석
	void ParsingReceiveData(FString& ParsedData, const TArray<uint8>& ReceivedData, int startNum = 0); // uint8 array의 데이터를 String에 붙이는 함수

private:
	static AProtocolLibrary* Instance;
	FSocket* Socket;
	FString PendingSendData;

	//2second 
	float ElapsedTime;

public:
	// singleton return
	static AProtocolLibrary* GetInstance(UWorld* World);

	// singleton Destory
	static void DestroyInstance();



	// now time + 0.1s
	FDateTime AddTime(FDateTime Time, float Seconds);

};
