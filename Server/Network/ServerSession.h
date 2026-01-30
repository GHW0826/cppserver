#pragma once
#ifndef SERVERSESSION_H
#define SERVERSESSION_H

#include "PacketSession.h"
#include <iostream>
#include "Types.h"
class ServerSession : public PacketSession
{
public:
	~ServerSession()
	{
		std::cout << "~ServerSession" << std::endl;
	}

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;
};

#endif // SERVERSESSION_H