#pragma once
#ifndef THREADMANAGER_H
#define THREADMANAGER_H

class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();
private:
	Mutex _lock;
};

#endif // THREADMANAGER_H

