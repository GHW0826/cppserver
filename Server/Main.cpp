#include "pch.h"
#include "ThreadManager.h"
#include <iostream>
#include "NetAddress.h"
#include "ServerSession.h"
#include "Service.h"
#include "IOCPCore.h"
#include "EpollCore.h"
#include "ClientPacketHandler.h"

enum {
	WORKER_TICK = 64
};

void DoWorkerJob(ServerServiceRef& service)
{
	while (true) {
		LEndTickCount = Tick::NowMs() + WORKER_TICK;

		// 네트워크 입출력 처리 -> 인게임 로직까지 (패킷 핸들러에 의해)
		service->GetNetCore()->Dispatch(10);

		// 예약된 일감 처리
		ThreadManager::DistributeReservedJobs();

		// 글로벌 큐
		ThreadManager::DoGlobalQueueWork();
	}
}

int main() {
	std::cout << "Server is running..." << std::endl;
	// Server logic goes here
	int8 t = 0;

	std::cout << CPP_VERSION << " " << __cplusplus << std::endl;

	//GThreadManager->Launch([]()
 //       {
	//		for (int i = 0; i < 1'000'000; ++i) {
	//			std::cout << "i: " << i << std::endl;
	//		}
 //       });

	ClientPacketHandler::Init();

	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<NetCore>(),
		MakeShared<ServerSession>,
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++) {
		GThreadManager->Launch([&service]() {
			DoWorkerJob(service);
		});
	}

	// Main Thread
	DoWorkerJob(service);

	GThreadManager->Join();

	return 0;
}