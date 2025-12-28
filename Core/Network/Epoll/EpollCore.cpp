#include "pch.h"

#if defined(__unix__) || defined(__unix) || defined(__linux__)
#include "EpollCore.h"

EpollCore::EpollCore()
	: _maxEvents(maxEvents)
{
	_epollHandle = epoll_create1(0);
	ASSERT_CRASH(_epollHandle != -1);

	_events.resize(_maxEvents);
}

EpollCore::~EpollCore()
{
	if (_epollHandle != -1)
		close(_epollHandle);
}

bool EpollCore::Register(NetObjectRef epollObject)
{
	return false;
}

bool EpollCore::Dispatch(uint32 timeoutMs)
{
	
	return true;
}

#endif