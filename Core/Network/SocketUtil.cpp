#include "pch.h"
#include "SocketUtil.h"

bool SocketUtil::SetReuseAddress(NetHandle socket, bool flag)
{
	return SetSocketOpt(socket, SOL_SOCKET, SO_REUSEADDR, (int)flag);
}

bool SocketUtil::SetRecvBufferSize(NetHandle socket, int32 size)
{
	return SetSocketOpt(socket, SOL_SOCKET, SO_RCVBUF, size);
}

bool SocketUtil::SetSendBufferSize(NetHandle socket, int32 size)
{
	return SetSocketOpt(socket, SOL_SOCKET, SO_SNDBUF, size);
}

bool SocketUtil::Bind(NetHandle socket, NetAddress netAddress)
{
	return 0 == ::bind(socket, reinterpret_cast<const SOCKADDR*>(&netAddress.GetSockAddr()), sizeof(SOCKADDR_IN));
}

bool SocketUtil::BindAnyAddress(NetHandle socket, uint16 port)
{
	SOCKADDR_IN address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = ::htonl(INADDR_ANY);
	address.sin_port = ::htons(port);

	return 0 == ::bind(socket, reinterpret_cast<const SOCKADDR*>(&address), sizeof(address));
}

bool SocketUtil::Listen(NetHandle socket, int32 backlog)
{
	return 0 == ::listen(socket, backlog);
}

