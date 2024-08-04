#include "ProtocolLibrary.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Misc/DateTime.h" //Time 
#include "HAL/PlatformProcess.h" // For FPlatformProcess::Sleep

AProtocolLibrary* AProtocolLibrary::Instance = nullptr;
bool bFirstTick = true;
int32 ActualBufferSize = 1048576; // 초기값 설정

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
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
	
	int32 BufferSize = 1048576; // 예시로 1MB 설정
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

	// 싱글톤 인스턴스 설정
	if (Instance == nullptr)
	{
		Instance = this;
	}

	// 서버에 연결 시도
	ConnectToServer(TEXT("127.0.0.1"), 12345);

	// 대기 시간 추가 (2초)
	//FPlatformProcess::Sleep(2.0f);

	// 서버가 준비되었는지 확인
	if (Socket && Socket->GetConnectionState() == SCS_Connected)
	{
		UE_LOG(LogTemp, Log, TEXT("Socket is connected and ready to send data."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Socket is not connected or ready."));
	}
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

	// 첫 번째 Tick에서 2초 대기
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

		//// 첫 번째 요청 후 응답 대기
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
	UE_LOG(LogTemp, Error, TEXT("Here"));

	TArray<uint8> ReceivedData;
	ReceivedData.SetNumUninitialized(ActualBufferSize);

	int32 TotalBytesRead = 0;
	int32 BytesRead = 0;
	const float Timeout = 5.0f; // 최대 대기 시간 (초)
	const float PollInterval = 0.1f; // 폴링 간격 (초)
	float TimeWaited = 0.0f;

	// 소켓을 넌블로킹 모드로 설정
	Socket->SetNonBlocking(true);

	while (TimeWaited < Timeout)
	{
		BytesRead = 0;
		bool bRecv = Socket->Recv(ReceivedData.GetData() + TotalBytesRead, ActualBufferSize - TotalBytesRead, BytesRead, ESocketReceiveFlags::None);
		if (bRecv && BytesRead > 0)
		{
			TotalBytesRead += BytesRead;
			break; // 데이터를 받으면 루프 종료
		}

		FPlatformProcess::Sleep(PollInterval);
		TimeWaited += PollInterval;
	}

	// 소켓을 다시 블로킹 모드로 설정
	Socket->SetNonBlocking(false);

	if (TotalBytesRead > 0)
	{
		FString ReceivedString = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(ReceivedData.GetData())));
		UE_LOG(LogTemp, Warning, TEXT("string cut!"));
		return ReceivedString.Left(TotalBytesRead);  // 실제 수신된 데이터 크기에 맞게 문자열 잘라내기
	}

	return FString();
}

void AProtocolLibrary::ParsingSL(FString& ParsedData, const TArray<uint8>& ReceivedData) {
	FString valueOfSL; // SL에서 해석한 길이를 받을 예정
	int next; // 다음 구분자가 있는 배열의 순번
	const int ALL_SEPARATOR_LENGTH = 2; // 모든 구분자의 문자는 2글자

	/*if (ReceivedData.Len() < 0) {
		UE_LOG(LogTemplateCharacter, Error, TEXT("---- ReceivedData is empty ----"));
	}*/

	// 3-2 - SL이외의 문자가 나올때까지 길이를 받음
	//    - 2번째 배열부터 검사
	//	  - 무조건 SL의 길이를 처음에 전부 받고, 이후의 SH의 일부값이 있다고 가정함 
	for (next = 2; (ReceivedData[next] > 47 && ReceivedData[next] < 58); next++) {
		valueOfSL.AppendChar(ReceivedData[next]);
	}

	// 3-3 앞으로 Parsing될 데이터를 담을 FString의 길이를 할당함
	ParsedData.Reserve(FCString::Atoi(*valueOfSL));

	// 3-4 for문을 통해서 이전의 문자열을 계속 넣어줌
	ParsingReceiveData(ParsedData, ReceivedData, (next+ALL_SEPARATOR_LENGTH));
}

// staratNum: 
void AProtocolLibrary::ParsingReceiveData(FString& ParsedData, const TArray<uint8>& ReceivedData, int startNum) {
	// 반복문을 통해서 받은 데이터를 ParsedData에 붙임
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

