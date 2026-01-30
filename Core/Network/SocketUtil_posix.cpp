#include "pch.h"
#include "SocketUtil.h"



#if defined(__unix__) || defined(__unix) || defined(__linux__)
#include <sys/socket.h> 
#include <netinet/tcp.h> 
#include <fcntl.h>      // fcntl, F_GETFL, F_SETFL, O_NONBLOCK
#include <unistd.h>     // close (필요시)
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>


int SocketUtil::SetNonBlocking(int32 fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) {
		return -1;
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		return -1;
	}
	return 0;
}

NetHandle SocketUtil::CreateSocket()
{
	return ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

void SocketUtil::Init()
{
}

void SocketUtil::Clear()
{
}

void SocketUtil::Close(NetHandle& socket)
{
	if (socket != -1)
		::close(socket);
	socket = -1;
}

bool SocketUtil::SetLinger(NetHandle socket, uint16 onoff, uint16 linger)
{
	struct linger option {};
	option.l_onoff = onoff ? 1 : 0;
	option.l_linger = linger; // 초 단위
	return SetSocketOpt(socket, SOL_SOCKET, SO_LINGER, option);
}

bool SocketUtil::SetNoDelay(NetHandle socket, bool flag)
{
	return SetSocketOpt(socket, IPPROTO_TCP, TCP_NODELAY, flag);
}

// acceptSocket의 특성을 ClientSocket에 그대로 적용
bool SocketUtil::SetUpdateAcceptSocket(NetHandle socket, NetHandle acceptSocket)
{
	return true;
}

#endif