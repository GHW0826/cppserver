#pragma once
#ifndef TYPES_H
#define TYPES_H

#include "Macros.h"

#include <mutex>
#include <thread>
#include <functional>
#include <memory>


using BYTE = std::uint8_t;
using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;
using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

template<typename T>
using Func = std::function<T>;
template<typename T>
using Callback = std::function<T>;

using VoidCallback = std::function<void()>;

using Mutex = std::mutex;
using Thread = std::thread;

template<typename T>
using Atomic = std::atomic<T>;
template<typename T>
using UniqueLock = std::unique_lock<T>;
template<typename T>
using LockGuard = std::lock_guard<T>;

template<typename T>
using SharedPtr = std::shared_ptr<T>;
template<typename T>
using WeakedPtr = std::weak_ptr<T>;
template<typename T>
using UniquePtr = std::unique_ptr<T>;


#if defined(_WIN32) || defined(_WIN64)
using Tick = struct Tick_Windows;
using HANDLETYPE = HANDLE;
using WideChar = WCHAR;
using WideString = std::wstring;
using NetHandle = SOCKET;

USING_SHARED_PTR(IOCPObject);
USING_SHARED_PTR(IOCPCore);
USING_SHARED_PTR(IOCPListener);
USING_SHARED_PTR(IOCPSession);

#endif

#if defined(__unix__) || defined(__unix) || defined(__linux__)
using Tick = struct Tick_Linux;
using HANDLETYPE = int32;
using WideString = std::string;
using WideChar = char;
using NetHandle = int32;

using SOCKADDR_IN = struct sockaddr_in;
using SOCKADDR = struct sockaddr;
using IN_ADDR = struct in_addr;

USING_SHARED_PTR(EpollObject);
USING_SHARED_PTR(EpollCore);
USING_SHARED_PTR(EpollListener);
USING_SHARED_PTR(EpollSession);

#endif


USING_SHARED_PTR(PacketSession);
USING_SHARED_PTR(Job);
USING_SHARED_PTR(JobQueue);
USING_SHARED_PTR(SendBuffer);
USING_SHARED_PTR(SendBufferChunk);
USING_SHARED_PTR(ServerService);
USING_SHARED_PTR(ClientService);


template<typename T>
using EnableSharedFromThis = std::enable_shared_from_this<T>;

#if defined(_WIN32) || defined(_WIN64)

using NetListenerRef = IOCPListenerRef;
using NetListener = IOCPListener;

using NetSession = IOCPSession;
using NetSessionRef = IOCPSessionRef;
using NetObjectRef = IOCPObjectRef;
using NetCoreRef = IOCPCoreRef;

using NetCore = IOCPCore;

#elif defined(__unix__) || defined(__unix) || defined(__linux__)

using NetListenerRef = EpollListenerRef;
using NetListener = EpollListener;

using NetSession = EpollSession;
using NetSessionRef = EpollSessionRef;
using NetObjectRef = EpollObjectRef;
using NetCoreRef = EpollCoreRef;

using NetCore = EpollCore;

#endif

using SessionFactory = std::function<NetSessionRef(void)>;

/*
#ifdef 0   // C++11 이전 계열
	#if defined(_WIN32) || defined(_WIN64)
		typedef unsigned char BYTE;
		typedef __int8  int8;
		typedef __int16 int16;
		typedef __int32 int32;
		typedef __int64 int64;
		typedef unsigned __int8  uint8;
		typedef unsigned __int16 uint16;
		typedef unsigned __int32 uint32;
		typedef unsigned __int64 uint64;
	#else // Linux, MacOS
		#include <stdint.h>

		typedef uint8_t BYTE;

		typedef int8_t  int8;
		typedef int16_t int16;
		typedef int32_t int32;
		typedef int64_t int64;
		typedef uint8_t  uint8;
		typedef uint16_t uint16;
		typedef uint32_t uint32;
		typedef uint64_t uint64;
	#endif // !defined(_WIN32) || defined(_WIN64)
#endif // Types
*/



#endif // !TYPES_H