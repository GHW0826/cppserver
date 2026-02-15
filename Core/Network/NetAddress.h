#pragma once
#ifndef NETADDRESS_H
#define NETADDRESS_H

#include "Types.h"

class NetAddress
{
public:
	NetAddress() = default;
	NetAddress(SOCKADDR_IN sockAddr);
	NetAddress(WideString ip, uint16 port);
public:
	SOCKADDR_IN& GetSockAddr() { return _sockAddr; }
	inline uint16 GetPort() { return static_cast<uint16>(::htons(_sockAddr.sin_port)); }
	WideString GetIpAddress();
public:
	static IN_ADDR Ip2Address(const WideChar* ip);
private:
	SOCKADDR_IN _sockAddr = {};
};

#endif
