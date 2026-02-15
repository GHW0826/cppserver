#include "pch.h"
#include "ClientPacketHandler.h"
#include "ServerSession.h"

PacketHandlerFunc GClientPacketHandler[UINT16_MAX];

bool Handle_C_TEST(PacketSessionRef& session, Protocol::C_TEST& pkt)
{
	// TODO : Log
	return true;
}

bool Handle_C_CONNECT(PacketSessionRef& session, Protocol::C_CONNECT& pkt)
{
	ServerSessionRef gameSession = static_pointer_cast<ServerSession>(session);

	std::cout << "Connect Client" << std::endl;

	Protocol::S_CONNECT loginPkt;
	loginPkt.set_success(true);
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPkt);
	session->Send(sendBuffer);

	return true;
}

bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	return true;
}