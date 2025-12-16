#include "pch.h"
#include <iostream>
#include "Service.h"
#include <thread>
#include "Types.h"
#include "Memory.h"
#include "Session.h"

class ServerSession : public Session
{

};

int main()
{
	std::cout << "Hello Test" << std::endl;

	// ServerPacketHandler::Init();

	// this_thread::sleep_for(1s);

	ClientServiceRef service = MakeShared<ClientService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<NetCore>(),
		MakeShared<ServerSession>,  // TODO : SessionManager µî
		1);

	ASSERT_CRASH(service->Start());

	return 0;
}