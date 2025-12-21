#pragma once
#ifndef COREPCH_H
#define COREPCH_H

#include "Types.h"
#include "Macros.h"
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
#include "Session.h"
#include "JobQueue.h"
#include "SendBufferManager.h"
#include "CorePacketHandler.h"


#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#endif // win

#endif // COREPCH_H