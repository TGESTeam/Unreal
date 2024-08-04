#include "ProtocolLibrary.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Misc/DateTime.h" //Time 
#include "HAL/PlatformProcess.h" // For FPlatformProcess::Sleep

AProtocolLibrary* AProtocolLibrary::Instance = nullptr;
bool bFirstTick = true;
int32 ActualBufferSize = 1048576; // �ʱⰪ ����

// ���� �ð��� 0.1�ʸ� ���ϴ� �޼���
FDateTime AProtocolLibrary::AddTime(FDateTime Time, float Seconds)
{
	return Time + FTimespan::FromSeconds(Seconds);
}

// Sets default values
AProtocolLibrary::AProtocolLibrary()
{
	PrimaryActorTick.bCanEverTick = true;

	// ���� �ʱ�ȭ
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
	
	int32 BufferSize = 1048576; // ���÷� 1MB ����
	Socket->SetReceiveBufferSize(BufferSize, ActualBufferSize);
	UE_LOG(LogTemp, Log, TEXT("Attempted to set receive buffer size to: %d"), BufferSize);
	UE_LOG(LogTemp, Log, TEXT("Actual receive buffer size set to: %d"), ActualBufferSize);
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

	// �̱��� �ν��Ͻ� ����
	if (Instance == nullptr)
	{
		Instance = this;
	}

	// ������ ���� �õ�
	ConnectToServer(TEXT("127.0.0.1"), 12345);

	// ��� �ð� �߰� (2��)
	//FPlatformProcess::Sleep(2.0f);

	// ������ �غ�Ǿ����� Ȯ��
	if (Socket && Socket->GetConnectionState() == SCS_Connected)
	{
		UE_LOG(LogTemp, Log, TEXT("Socket is connected and ready to send data."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Socket is not connected or ready."));
	}
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

// �̱��� �ν��Ͻ��� ��������� �����ϴ� �Լ�
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

	// ù ��° Tick���� 2�� ���
	//if (bFirstTick)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("Waiting for 2 seconds before first data send..."));
	//	FPlatformProcess::Sleep(1.0f);
	//	bFirstTick = false;
	//}



	ElapsedTime += DeltaTime;
	if (ElapsedTime >= 2.0f )
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

		UE_LOG(LogTemp, Log, TEXT("send reuest: %s"), *string);
		SendData(string);

		//// ù ��° ��û �� ���� ���
		//if (bFirstTick)
		//{
		//	UE_LOG(LogTemp, Log, TEXT("Waiting for response after first data send..."));
		//	FPlatformProcess::Sleep(5.0f);
		//	bFirstTick = false;
		//}

		FString Response = ReceiveData();
		//UE_LOG(LogTemp, Log, TEXT("parse string : %s"), *string);
		UE_LOG(LogTemp, Log, TEXT("Received response: %s"), *Response);
	}

	// PlayerLocation value print -
	//PrintPlayerLocation();
	//PrintPlayerViewDirection();

	// ���� �ð� ������Ʈ �� ���
	//CurrentTime = FDateTime::UtcNow();
	//CurrentTime = AddTime(CurrentTime, 0.1);
	//UE_LOG(LogTemp, Log, TEXT("Current Time: %s"), *CurrentTime.ToString());

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
	if (!Socket) return;

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
	UE_LOG(LogTemp, Error, TEXT("Here"));

	TArray<uint8> ReceivedData;
	ReceivedData.SetNumUninitialized(ActualBufferSize);

	int32 TotalBytesRead = 0;
	int32 BytesRead = 0;
	const float Timeout = 5.0f; // �ִ� ��� �ð� (��)
	const float PollInterval = 0.1f; // ���� ���� (��)
	float TimeWaited = 0.0f;

	// ������ �ͺ��ŷ ���� ����
	Socket->SetNonBlocking(true);

	while (TimeWaited < Timeout)
	{
		BytesRead = 0;
		bool bRecv = Socket->Recv(ReceivedData.GetData() + TotalBytesRead, ActualBufferSize - TotalBytesRead, BytesRead, ESocketReceiveFlags::None);
		if (bRecv && BytesRead > 0)
		{
			TotalBytesRead += BytesRead;
			break; // �����͸� ������ ���� ����
		}

		FPlatformProcess::Sleep(PollInterval);
		TimeWaited += PollInterval;
	}

	// ������ �ٽ� ���ŷ ���� ����
	Socket->SetNonBlocking(false);

	if (TotalBytesRead > 0)
	{
		FString ReceivedString = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(ReceivedData.GetData())));
		UE_LOG(LogTemp, Warning, TEXT("string cut!"));
		return ReceivedString.Left(TotalBytesRead);  // ���� ���ŵ� ������ ũ�⿡ �°� ���ڿ� �߶󳻱�
	}

	return FString();
}

void AProtocolLibrary::ParsingSL(FString& ParsedData, const TArray<uint8>& ReceivedData) {
	FString valueOfSL; // SL���� �ؼ��� ���̸� ���� ����
	int next; // ���� �����ڰ� �ִ� �迭�� ����
	const int ALL_SEPARATOR_LENGTH = 2; // ��� �������� ���ڴ� 2����

	/*if (ReceivedData.Len() < 0) {
		UE_LOG(LogTemplateCharacter, Error, TEXT("---- ReceivedData is empty ----"));
	}*/

	// 3-2 - SL�̿��� ���ڰ� ���ö����� ���̸� ����
	//    - 2��° �迭���� �˻�
	//	  - ������ SL�� ���̸� ó���� ���� �ް�, ������ SH�� �Ϻΰ��� �ִٰ� ������ 
	for (next = 2; (ReceivedData[next] > 47 && ReceivedData[next] < 58); next++) {
		valueOfSL.AppendChar(ReceivedData[next]);
	}

	// 3-3 ������ Parsing�� �����͸� ���� FString�� ���̸� �Ҵ���
	ParsedData.Reserve(FCString::Atoi(*valueOfSL));

	// 3-4 for���� ���ؼ� ������ ���ڿ��� ��� �־���
	ParsingReceiveData(ParsedData, ReceivedData, (next+ALL_SEPARATOR_LENGTH));
}

// staratNum: 
void AProtocolLibrary::ParsingReceiveData(FString& ParsedData, const TArray<uint8>& ReceivedData, int startNum) {
	// �ݺ����� ���ؼ� ���� �����͸� ParsedData�� ����
	for (uint8 data : ReceivedData) {
		ParsedData.AppendChar(data);
	}

	UE_LOG(LogTemp, Log, TEXT("---------- Now String length: %d | val: %s"), ParsedData.GetAllocatedSize(), *ParsedData);
}




//FString AProtocolLibrary::ReceiveData()
//{
//	if (!Socket) return FString();
//	UE_LOG(LogTemp, Error, TEXT("Here"));
//	TArray<uint8> ReceivedData;
//	uint32 Size;
//
//	while (Socket->HasPendingData(Size))
//	{
//		UE_LOG(LogTemp, Error, TEXT("Here2"));
//		ReceivedData.SetNumUninitialized(FMath::Min(Size, (uint32)ActualBufferSize));
//
//		int32 Read = 0;
//		Socket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);
//	}
//
//	FString ReceivedString = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(ReceivedData.GetData())));
//	return ReceivedString;
//}

