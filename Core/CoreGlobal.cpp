#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"

ThreadManager* GThreadManager = nullptr;

class CoreGlobal
{
public:
	CoreGlobal();
	~CoreGlobal();
} GCoreGlobal;

CoreGlobal::CoreGlobal()
{
	RAW_POINTER_NEW(GThreadManager, ThreadManager);
}

CoreGlobal::~CoreGlobal()
{
	RAW_POINTER_DELETE(GThreadManager);
}
