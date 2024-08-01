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

	//변수
	struct LOVector {
		double X;
		double Y;
		double Z;
	};

	LOVector PlayerLocation;


	// PlayerLocation 값을 출력하는 함수
	void PrintPlayerLocation() const;

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

public:
	// 싱글톤 인스턴스를 반환하는 함수
	static AProtocolLibrary* GetInstance(UWorld* World);

};
