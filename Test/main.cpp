#include "pch.h"
#include <iostream>
#include "Service.h"
#include <thread>
#include "Types.h"
#include "Memory.h"
#include "PacketSession.h"
#include "IOCPCore.h"
#include "ServerPacketHandler.h"
#include "ThreadManager.h"
#include <thread>
#include <chrono>

char sendData[] = "Hello World";

class ServerSession : public PacketSession
{
public:
	~ServerSession()
	{
		std::cout << "~ServerSession" << std::endl;
	}

	virtual void OnConnected() override
	{
		Protocol::C_CONNECT pkt;
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		Send(sendBuffer);
	}

	virtual void OnRecvPacket(BYTE* buffer, int32 len) override
	{
		PacketSessionRef session = GetPacketSessionRef();
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

		// TODO : packetId 대역 체크
		ServerPacketHandler::HandlePacket(session, buffer, len);
	}

	virtual void OnSend(int32 len) override
	{
		//cout << "OnSend Len = " << len << endl;
	}

	virtual void OnDisconnected() override
	{
		//cout << "Disconnected" << endl;
	}
};


int main()
{
	std::cout << "Hello Test" << std::endl;

	ServerPacketHandler::Init();

	// this_thread::sleep_for(1s);

	ClientServiceRef service = MakeShared<ClientService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<NetCore>(),
		MakeShared<ServerSession>,  // TODO : SessionManager 등
		1);

	ASSERT_CRASH(service->Start());
	for (int32 i = 0; i < 2; i++) {
		GThreadManager->Launch([=]() {
				while (true) {
					service->GetNetCore()->Dispatch();
				}
			});
	}


	Protocol::C_CHAT chatPkt;
	chatPkt.set_msg("Hello World !");
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(chatPkt);

	while (true) {
		service->Broadcast(sendBuffer);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	GThreadManager->Join();


	return 0;
}