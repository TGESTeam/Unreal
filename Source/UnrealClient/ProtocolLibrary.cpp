#include "ProtocolLibrary.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Misc/DateTime.h" //Time 

AProtocolLibrary* AProtocolLibrary::Instance = nullptr;

// 현재 시간에 0.1초를 더하는 메서드
FDateTime AProtocolLibrary::AddTime(FDateTime Time, float Seconds)
{
	return Time + FTimespan::FromSeconds(Seconds);
}

// Sets default values
AProtocolLibrary::AProtocolLibrary()
{
	PrimaryActorTick.bCanEverTick = true;

	// 소켓 초기화
	//Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
}

AProtocolLibrary::~AProtocolLibrary()
{
	if (Socket)
	{
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
		Socket = nullptr;
	}
}

void AProtocolLibrary::PrintPlayerLocation() const
{
	UE_LOG(LogTemp, Log, TEXT("Player Location: X=%lf, Y=%lf, Z=%lf"), PlayerLocation.X, PlayerLocation.Y, PlayerLocation.Z);
}

void AProtocolLibrary::PrintPlayerViewDirection() const
{
	UE_LOG(LogTemp, Log, TEXT("Player View Direction: X=%lf, Y=%lf, Z=%lf"), PlayerViewDirection.Pitch, PlayerViewDirection.Yaw, PlayerViewDirection.Roll);
}

// Called when the game starts or when spawned
void AProtocolLibrary::BeginPlay()
{
	Super::BeginPlay();

	// 싱글톤 인스턴스 설정
	if (Instance == nullptr)
	{
		Instance = this;
	}

	// 서버에 연결 시도
	//ConnectToServer(TEXT("127.0.0.1"), 12345);
}

// 싱글톤 인스턴스를 반환하는 함수
AProtocolLibrary* AProtocolLibrary::GetInstance(UWorld* World)
{
	if (Instance == nullptr)
	{
		Instance = World->SpawnActor<AProtocolLibrary>(AProtocolLibrary::StaticClass());
	}
	return Instance;
}

// 싱글톤 인스턴스를 명시적으로 해제하는 함수
void AProtocolLibrary::DestroyInstance()
{
	if (Instance != nullptr)
	{
		Instance->Destroy();
		Instance = nullptr;
	}
}



// Called every frame
void AProtocolLibrary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;
	if (ElapsedTime >= 2.0f)
	{
		ElapsedTime = 0.0f;

		FString string;
		//1. SH -> VL size
		string += "SH";
		string += "2,3,5,5";

		//2. playLoction -> LO
		string += "LO";
		string += FString::Printf(TEXT("%lf"), PlayerLocation.X) + "," \
			+ FString::Printf(TEXT("%lf"), PlayerLocation.Y) + "," \
			+ FString::Printf(TEXT("%lf"), PlayerLocation.Z);
		//3. playViewDirection -> RO
		string += "RO";
		string += FString::Printf(TEXT("%lf"), PlayerViewDirection.Pitch) + "," \
			+ FString::Printf(TEXT("%lf"), PlayerViewDirection.Yaw) + "," \
			+ FString::Printf(TEXT("%lf"), PlayerViewDirection.Roll);

		//4. TI -> TI
		string += "TI";
		CurrentTime = FDateTime::UtcNow();
		CurrentTime = AddTime(CurrentTime, 0.1);
		string += *CurrentTime.ToString();

		UE_LOG(LogTemp, Log, TEXT("parse string : %s"), *string);
	}

	// PlayerLocation value print -
	//PrintPlayerLocation();
	//PrintPlayerViewDirection();

	// 현재 시간 업데이트 및 출력
	//CurrentTime = FDateTime::UtcNow();
	//CurrentTime = AddTime(CurrentTime, 0.1);
	//UE_LOG(LogTemp, Log, TEXT("Current Time: %s"), *CurrentTime.ToString());

	// 데이터를 수신하고 처리
	//FString ReceivedData = ReceiveData();
	//if (!ReceivedData.IsEmpty())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Received data: %s"), *ReceivedData);
	//}

	//// 대기 중인 데이터를 전송
	//if (!PendingSendData.IsEmpty())
	//{
	//	SendData(PendingSendData);
	//	PendingSendData.Empty();
	//}
}

void AProtocolLibrary::ConnectToServer(const FString& ServerAddress, int32 Port)
{
	if (!Socket) return;

	// 서버 주소와 포트 설정
	FIPv4Address IP;
	FIPv4Address::Parse(ServerAddress, IP);
	TSharedRef<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Addr->SetIp(IP.Value);
	Addr->SetPort(Port);

	// 서버에 연결
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
