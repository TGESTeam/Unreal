#include "ProtocolLibrary.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

AProtocolLibrary* AProtocolLibrary::Instance = nullptr;

// Sets default values
AProtocolLibrary::AProtocolLibrary()
{
	PrimaryActorTick.bCanEverTick = true;

	// ���� �ʱ�ȭ
	//Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
}

void AProtocolLibrary::PrintPlayerLocation() const
{
	UE_LOG(LogTemp, Log, TEXT("Player Location: X=%lf, Y=%lf, Z=%lf"), PlayerLocation.X, PlayerLocation.Y, PlayerLocation.Z);
}

// Called when the game starts or when spawned
void AProtocolLibrary::BeginPlay()
{
	Super::BeginPlay();

	// �̱��� �ν��Ͻ� ����
	if (Instance == nullptr)
	{
		Instance = this;
	}

	// ������ ���� �õ�
	//ConnectToServer(TEXT("127.0.0.1"), 12345);
}

// �̱��� �ν��Ͻ��� ��ȯ�ϴ� �Լ�
AProtocolLibrary* AProtocolLibrary::GetInstance(UWorld* World)
{
	if (Instance == nullptr)
	{
		Instance = World->SpawnActor<AProtocolLibrary>(AProtocolLibrary::StaticClass());
	}
	return Instance;
}


// Called every frame
void AProtocolLibrary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// PlayerLocation ���� ���
	PrintPlayerLocation();

	// �����͸� �����ϰ� ó��
	//FString ReceivedData = ReceiveData();
	//if (!ReceivedData.IsEmpty())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Received data: %s"), *ReceivedData);
	//}

	//// ��� ���� �����͸� ����
	//if (!PendingSendData.IsEmpty())
	//{
	//	SendData(PendingSendData);
	//	PendingSendData.Empty();
	//}
}

void AProtocolLibrary::ConnectToServer(const FString& ServerAddress, int32 Port)
{
	// ���� �ּҿ� ��Ʈ ����
	FIPv4Address IP;
	FIPv4Address::Parse(ServerAddress, IP);
	TSharedRef<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Addr->SetIp(IP.Value);
	Addr->SetPort(Port);

	// ������ ����
	bool Connected = Socket->Connect(*Addr);
	if (Connected)
	{
		UE_LOG(LogTemp, Warning, TEXT("Connected to server!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to connect to server."));
	}
}

void AProtocolLibrary::SendData(const FString& Data)
{
	if (!Socket) return;

	auto Serialized = StringCast<ANSICHAR>(*Data);
	int32 Size = FCStringAnsi::Strlen(Serialized.Get()) + 1;
	int32 Sent = 0;

	bool Successful = Socket->Send((uint8*)Serialized.Get(), Size, Sent);
	if (Successful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Data sent successfully."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to send data."));
	}
}

FString AProtocolLibrary::ReceiveData()
{
	if (!Socket) return FString();

	TArray<uint8> ReceivedData;
	uint32 Size;
	while (Socket->HasPendingData(Size))
	{
		ReceivedData.SetNumUninitialized(FMath::Min(Size, 65507u));

		int32 Read = 0;
		Socket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);
	}

	FString ReceivedString = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(ReceivedData.GetData())));
	return ReceivedString;
}
