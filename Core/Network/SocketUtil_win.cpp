#include "pch.h"
#include "SocketUtil.h"


#if defined(_WIN32) || defined(_WIN64)

LPFN_CONNECTEX SocketUtil::ConnectEx = nullptr;
LPFN_DISCONNECTEX SocketUtil::DisconnectEx = nullptr;
// AcceptEx는 미리 만들어둔 소켓에 새로 들어온 연결을 붙여주는(attach) 방식
LPFN_ACCEPTEX SocketUtil::AcceptEx = nullptr;

void SocketUtil::Init()
{
	WSADATA wsaData;
	ASSERT_CRASH(::WSAStartup(MAKEWORD(2, 2), OUT & wsaData) == 0);

	// 런타임에 주소 얻어오는 API
	SOCKET dummySocket = CreateSocket();
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)));
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)));
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)));
	Close(dummySocket);
}

void SocketUtil::Clear()
{
	::WSACleanup();
}

NetHandle SocketUtil::CreateSocket()
{
	return ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

bool SocketUtil::BindWindowsFunction(NetHandle socket, GUID guid, LPVOID* fn)
{
	DWORD bytes = 0;
	return SOCKET_ERROR != ::WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT & bytes, NULL, NULL);
}


void SocketUtil::Close(NetHandle& socket)
{
	if (socket != SOCKET_ERROR)
		::closesocket(socket);
	socket = INVALID_SOCKET;
}

bool SocketUtil::SetLinger(NetHandle socket, uint16 onoff, uint16 linger)
{
	LINGER option;
	option.l_onoff = onoff;
	option.l_linger = linger;
	return SetSocketOpt(socket, SOL_SOCKET, SO_LINGER, option);
}

bool SocketUtil::SetNoDelay(NetHandle socket, bool flag)
{
	return SetSocketOpt(socket, SOL_SOCKET, TCP_NODELAY, flag);
}

// acceptSocket의 특성을 ClientSocket에 그대로 적용
bool SocketUtil::SetUpdateAcceptSocket(NetHandle socket, NetHandle acceptSocket)
{
	return SetSocketOpt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, acceptSocket);
}


#endif