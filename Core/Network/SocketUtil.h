#pragma once
#ifndef SOCKETTUTIL_H
#define SOCKETTUTIL_H

#include "NetAddress.h"

class SocketUtil
{
public:
#if defined(_WIN32) || defined(_WIN64)
	static LPFN_CONNECTEX ConnectEx;
	static LPFN_DISCONNECTEX DisconnectEx;
	static LPFN_ACCEPTEX AcceptEx;
#endif
public:
	static void Init();
	static void Clear();
#if defined(_WIN32) || defined(_WIN64)
	static bool BindWindowsFunction(NetHandle socket, GUID guid, LPVOID* fn);
#endif
	static NetHandle CreateSocket();

	static bool SetLinger(NetHandle socket, uint16 onoff, uint16 linger);
	static bool SetReuseAddress(NetHandle socket, bool flag);
	static bool SetRecvBufferSize(NetHandle socket, int32 size);
	static bool SetSendBufferSize(NetHandle socket, int32 size);
	static bool SetNoDelay(NetHandle socket, bool flag);
	static bool SetUpdateAcceptSocket(NetHandle socket, NetHandle acceptSocket);

	static bool Bind(NetHandle socket, NetAddress netAddress);
	static bool BindAnyAddress(NetHandle socket, uint16 port);
	static bool Listen(NetHandle socket, int32 backlog = SOMAXCONN);
	static void Close(NetHandle& socket);
#if defined(__unix__) || defined(__unix) || defined(__linux__)
	static int SetNonBlocking(int32 fd);
#endif
};


#if defined(__unix__) || defined(__unix) || defined(__linux__)

template<typename T>
static inline bool SetSocketOpt(NetHandle socket, int32 level, int32 optName, T optValue)
{
	return 0 == ::setsockopt((int)socket, level, optName, &optValue, sizeof(T));
}
#endif

#if defined(_WIN32) || defined(_WIN64)

template<typename T>
static inline bool SetSocketOpt(NetHandle socket, int32 level, int32 optName, T optValue)
{
	return 0 == ::setsockopt(socket, level, optName, reinterpret_cast<char*>(&optValue), sizeof(T));
}

#endif

#endif // SOCKETTUTIL_H
