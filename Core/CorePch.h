#pragma once
#ifndef COREPCH_H
#define COREPCH_H

#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include "IOCPSession.h"

#else
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#endif // win

#include "Types.h"
#include "Macros.h"
#include "Structs.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"
#include "Container.h"

#include <stdint.h>
#include <iostream>

#include "CustomLock.h"
#include "CustomReadLockGuard.h"
#include "CustomWriteLockGuard.h"
#include "Structs.h"
#include "ObjectPool.h"
#include "Memory.h"
#include "SendBuffer.h"
#include "JobQueue.h"
#include "SendBufferManager.h"
#include "CorePacketHandler.h"

#endif // COREPCH_H