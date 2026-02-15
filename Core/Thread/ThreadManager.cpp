#include "pch.h"
#include "ThreadManager.h"
#include "GlobalQueue.h"

ThreadManager::ThreadManager()
{
	InitTLS();
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Launch(Func<void()> callback)
{
	LockGuard<Mutex> guard(_lock);
	_threads.emplace_back(Thread([=]()
		{
			InitTLS();
			callback();
			DestroyTLS();
		}));
}

void ThreadManager::Join()
{
	for (Thread& t : _threads) {
		if (t.joinable())
			t.join();
	}
	_threads.clear();
}

void ThreadManager::InitTLS()
{
	static Atomic<uint32> SThreadId = 0;
	LThreadId = SThreadId.fetch_add(1);
}

void ThreadManager::DestroyTLS()
{
}

void ThreadManager::DoGlobalQueueWork()
{
	while (true) {
		uint64 now = Tick::NowMs();
		if (now > LEndTickCount)
			break;

		JobQueueRef jobQueue = GGlobalQueue->Pop();
		if (jobQueue == nullptr)
			break;
	}
}

void ThreadManager::DistributeReservedJobs()
{
	const uint64 now = Tick::NowMs();

	GJobTimer->Distribute(now);
}
