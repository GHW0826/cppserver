#include "pch.h"
#include "SocketUtil.h"


LPFN_CONNECTEX SocketUtil::ConnectEx = nullptr;
LPFN_DISCONNECTEX SocketUtil::DisconnectEx = nullptr;
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

bool SocketUtil::BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn)
{
	DWORD bytes = 0;
	return SOCKET_ERROR != ::WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT & bytes, NULL, NULL);
}

SOCKET SocketUtil::CreateSocket()
{
	return ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

bool SocketUtil::SetLinger(SOCKET socket, uint16 onoff, uint16 linger)
{
	LINGER option;
	option.l_onoff = onoff;
	option.l_linger = linger;
	return SetSocketOpt(socket, SOL_SOCKET, SO_LINGER, option);
}

bool SocketUtil::SetReuseAddress(SOCKET socket, bool flag)
{
	return SetSocketOpt(socket, SOL_SOCKET, SO_REUSEADDR, flag);
}

bool SocketUtil::SetRecvBufferSize(SOCKET socket, int32 size)
{
	return SetSocketOpt(socket, SOL_SOCKET, SO_RCVBUF, size);
}

bool SocketUtil::SetSendBufferSize(SOCKET socket, int32 size)
{
	return SetSocketOpt(socket, SOL_SOCKET, SO_SNDBUF, size);
}

bool SocketUtil::SetNoDelay(SOCKET socket, bool flag)
{
	return SetSocketOpt(socket, SOL_SOCKET, TCP_NODELAY, flag);
}

// acceptSocket의 특성을 ClientSocket에 그대로 적용
bool SocketUtil::SetUpdateAcceptSocket(SOCKET socket, SOCKET acceptSocket)
{
	return SetSocketOpt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, acceptSocket);
}

bool SocketUtil::Bind(SOCKET socket, NetAddress netAddress)
{
	return SOCKET_ERROR != ::bind(socket, reinterpret_cast<const SOCKADDR*>(&netAddress.GetSockAddr()), sizeof(SOCKADDR_IN));
}

bool SocketUtil::BindAnyAddress(SOCKET socket, uint16 port)
{
	SOCKADDR_IN address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = ::htonl(INADDR_ANY);
	address.sin_port = ::htons(port);

	return SOCKET_ERROR != ::bind(socket, reinterpret_cast<const SOCKADDR*>(&address), sizeof(address));
}

bool SocketUtil::Listen(SOCKET socket, int32 backlog)
{
	return SOCKET_ERROR != ::listen(socket, backlog);
}

void SocketUtil::Close(SOCKET& socket)
{
	if (socket != INVALID_SOCKET)
		::closesocket(socket);
	socket = INVALID_SOCKET;
}
