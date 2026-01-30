#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "DeadLockProfiler.h"
#include "SocketUtil.h"
#include "JobTimer.h"
#include "GlobalQueue.h"
#include "SendBufferManager.h"
#include "Memory.h"
#include "CorePacketHandler.h"

ThreadManager* GThreadManager = nullptr;
DeadLockProfiler* GDeadLockProfiler = nullptr;
SendBufferManager* GSendBufferManager = nullptr;
Memory* GMemory = nullptr;
GlobalQueue* GGlobalQueue = nullptr;
JobTimer* GJobTimer = nullptr;

class CoreGlobal
{
public:
	CoreGlobal();
	~CoreGlobal();
} GCoreGlobal;

CoreGlobal::CoreGlobal()
{
	RAW_POINTER_NEW(GThreadManager, ThreadManager);
	//RAW_POINTER_NEW(GDeadLockProfiler, DeadLockProfiler);
	RAW_POINTER_NEW(GMemory, Memory);
	RAW_POINTER_NEW(GJobTimer, JobTimer);
	RAW_POINTER_NEW(GGlobalQueue, GlobalQueue);
	RAW_POINTER_NEW(GSendBufferManager, SendBufferManager);
	CorePacketHandler::Init();
	SocketUtil::Init();
}

CoreGlobal::~CoreGlobal()
{
	RAW_POINTER_DELETE(GThreadManager);
	//RAW_POINTER_DELETE(GDeadLockProfiler);
	RAW_POINTER_DELETE(GJobTimer);
	RAW_POINTER_DELETE(GMemory);
	RAW_POINTER_DELETE(GGlobalQueue);
	RAW_POINTER_DELETE(GSendBufferManager);
	SocketUtil::Clear();
}
