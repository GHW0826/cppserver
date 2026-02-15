#pragma once
#ifndef IOCPOBJECT_H
#define IOCPOBJECT_H

#include "Types.h"

#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>

class IOCPObject : public EnableSharedFromThis<IOCPObject>
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IOCPEvent* iocpEvent, int32 numOfBytes = 0) abstract;
};

#endif // win
#endif // IOCPBOBJECT_H
