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

	//º¯¼ö
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
