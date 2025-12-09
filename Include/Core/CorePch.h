#pragma once
#ifndef COREPCH_H
#define COREPCH_H

#include <memory>
#include <mutex>
#include <thread>
#include <functional>
#include <iostream>

#include "Types.h"
#include "Macros.h"
#include "Container.h"
#include "CoreGlobal.h"
#include "CoreTLS.h"
#include "CustomLock.h"
#include "CustomReadLockGuard.h"
#include "CustomWriteLockGuard.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif // win

#endif // COREPCH_H