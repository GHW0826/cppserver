#pragma once
#ifndef PACKETSESSION_H
#define PACKETSESSION_H

#if defined(_WIN32) || defined(_WIN64)
#include "IOCPSession.h"
#endif

#if defined(__unix__) || defined(__unix) || defined(__linux__)
#include "EpollSession.h"
#endif


class PacketSession : public NetSession
{
public:
	PacketSession();
	virtual ~PacketSession();

	PacketSessionRef	GetPacketSessionRef() { return static_pointer_cast<PacketSession>(shared_from_this()); }

protected:
	virtual int32		OnRecv(BYTE* buffer, int32 len) final;
	virtual void		OnRecvPacket(BYTE* buffer, int32 len) = 0;
};

#endif // PACKETSESSION_H