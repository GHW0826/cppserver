#pragma once
#ifndef IOCPCORE_H
#define IOCPCORE_H

#if defined(_WIN32) || defined(_WIN64)

#include "NetCore.h"

class IOCPCore
{
public:
	IOCPCore();
	~IOCPCore();
public:
	inline HANDLE GetHandle() { return _iocpHandle; }
	bool Register(IOCPObjectRef iocpObject);
	bool Dispatch(uint32 timeoutMs = INFINITE);
private:
	HANDLE _iocpHandle;
};

#endif // WIN

#endif // IOCPCORE_H