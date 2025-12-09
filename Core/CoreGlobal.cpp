#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "DeadLockProfiler.h"

ThreadManager* GThreadManager = nullptr;
DeadLockProfiler* GDeadLockProfiler = nullptr;

class CoreGlobal
{
public:
	CoreGlobal();
	~CoreGlobal();
} GCoreGlobal;

CoreGlobal::CoreGlobal()
{
	RAW_POINTER_NEW(GThreadManager, ThreadManager);
	RAW_POINTER_NEW(GDeadLockProfiler, DeadLockProfiler);
}

CoreGlobal::~CoreGlobal()
{
	RAW_POINTER_DELETE(GThreadManager);
	RAW_POINTER_DELETE(GDeadLockProfiler);
}
