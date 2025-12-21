#include "pch.h"
#include "ClientPacketHandler.h"

PacketHandlerFunc GClientPacketHandler[UINT16_MAX];

bool Handle_C_TEST(PacketSessionRef& session, Protocol::C_TEST& pkt)
{
	// TODO : Log
	return false;
}