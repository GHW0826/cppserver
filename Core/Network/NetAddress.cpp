#include "pch.h"
#include "NetAddress.h"

NetAddress::NetAddress(SOCKADDR_IN sockAddr)
	: _sockAddr(sockAddr)
{
}

NetAddress::NetAddress(WideString ip, uint16 port)
{
	::memset(&_sockAddr, 0, sizeof(_sockAddr));
	_sockAddr.sin_family = AF_INET;
	_sockAddr.sin_addr = Ip2Address(ip.c_str());
	_sockAddr.sin_port = ::htons(port);
}


#if defined(_WIN32) || defined(_WIN64)

WideString NetAddress::GetIpAddress()
{
	WideChar buffer[100];
	::InetNtopW(AF_INET, &_sockAddr.sin_addr, buffer, len32(buffer));
	return WideString(buffer);
}

IN_ADDR NetAddress::Ip2Address(const WideChar* ip)
{
	IN_ADDR address;
	::InetPtonW(AF_INET, ip, &address);
	return address;
}

#endif

#if defined(__unix__) || defined(__unix) || defined(__linux__)

WideString NetAddress::GetIpAddress()
{
	WideChar buffer[100];
	::inet_ntop(AF_INET, &_sockAddr.sin_addr, buffer, len32(buffer));
	return WideString(buffer);
}

IN_ADDR NetAddress::Ip2Address(const WideChar* ip)
{
	IN_ADDR address;
	::inet_pton(AF_INET, ip, &address);
	return address;
}

#endif