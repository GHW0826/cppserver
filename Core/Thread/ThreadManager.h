#pragma once
#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include "Container.h"
#include "Types.h"

class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();
public:
	void Launch(Func<void()> callback);
	void Join();

	static void InitTLS();
	static void DestroyTLS();

	static void DoGlobalQueueWork();
	static void DistributeReservedJobs();
private:
	Mutex _lock;
	Vector<Thread> _threads;
};

#endif // THREADMANAGER_H

