#pragma once
#ifndef EPOLLCORE_H
#define EPOLLCORE_H

#if defined(__unix__) || defined(__unix) || defined(__linux__)

#include "NetCore.h"

class EpollCore
{
public:
	EpollCore(int32 maxEvents);
	~EpollCore();
public:
	inline int32 GetHandle() { return _epollHandle; }
	bool Register(NetObjectRef epollObject);
	bool Dispatch(uint32 timeoutMs = INFINITE);
private:
	int32 _epollHandle = -1;
	int32 _maxEvents = 0;
	std::vector<epoll_event> _events;
};

#endif

#endif // EPOLLCORE_H
