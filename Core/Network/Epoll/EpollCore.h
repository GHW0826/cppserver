#pragma once
#ifndef EPOLLCORE_H
#define EPOLLCORE_H

#if defined(__unix__) || defined(__unix) || defined(__linux__)
#include <sys/epoll.h>
#include <sys/socket.h>  
#include "NetCore.h"
class EpollCore
{
public:
	EpollCore();
	~EpollCore();
public:
	inline int32 GetHandle() { return _epollHandle; }
	bool Register(NetObjectRef epollObject);
	bool Dispatch(uint32 timeoutMs = 0xFFFFFFFF /* INFINITE */);
private:
	int32 _epollHandle = -1;
	int32 _maxEvents = 4096;

	// 발생한 이벤트 보관용
	Vector<epoll_event> _events;
};

#endif

#endif // EPOLLCORE_H
