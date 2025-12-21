#include "pch.h"
#include "CorePacketHandler.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_C_HEALTHCHECK(PacketSessionRef& session, Protocol::C_HEALTHCHECK& pkt)
{
	// TODO : Log
	return false;
}

bool Handle_C_ECHO(PacketSessionRef& session, Protocol::C_ECHO& pkt)
{
	// TODO : Log
	return false;
}