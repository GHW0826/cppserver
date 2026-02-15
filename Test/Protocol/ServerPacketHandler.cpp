#include "pch.h"
#include "ServerPacketHandler.h"


bool Handle_S_TEST(PacketSessionRef& session, Protocol::S_TEST& pkt)
{
	return true;
}

bool Handle_S_CONNECT(PacketSessionRef& session, Protocol::S_CONNECT& pkt)
{
	if (pkt.success() == false)
		return true;

	std::cout << "Connect Server" << std::endl;


	return true;
}

bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	return true;
}