#pragma once
#include "Protocol.pb.h"

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

enum : uint16
{
	PKT_C_HEALTHCHECK = 1000,
	PKT_S_HEALTHCHECK = 1001,
	PKT_C_ECHO = 1002,
	PKT_S_ECHO = 1003,
};

// Custom Handlers
bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_C_HEALTHCHECK(PacketSessionRef& session, Protocol::C_HEALTHCHECK& pkt);
bool Handle_C_ECHO(PacketSessionRef& session, Protocol::C_ECHO& pkt);

class CorePacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[PKT_C_HEALTHCHECK] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_HEALTHCHECK>(Handle_C_HEALTHCHECK, session, buffer, len); };
		GPacketHandler[PKT_C_ECHO] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_ECHO>(Handle_C_ECHO, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	static SendBufferRef MakeSendBuffer(Protocol::S_HEALTHCHECK& pkt) { return MakeSendBuffer(pkt, PKT_S_HEALTHCHECK); }
	static SendBufferRef MakeSendBuffer(Protocol::S_ECHO& pkt) { return MakeSendBuffer(pkt, PKT_S_ECHO); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		SendBufferRef sendBuffer = GSendBufferManager->Open(packetSize);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		ASSERT_CRASH(pkt.SerializeToArray(&header[1], dataSize));
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};