#pragma once
#include "CSProtocol.pb.h"
#include "CorePacketHandler.h"

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;

enum : uint16
{
	PKT_C_TEST = 1000,
	PKT_S_TEST = 1001,
	PKT_C_CONNECT = 1002,
	PKT_S_CONNECT = 1003,
	PKT_C_CHAT = 1004,
	PKT_S_CHAT = 1005,
};

// Custom Handlers
bool Handle_S_TEST(PacketSessionRef& session, Protocol::S_TEST& pkt);
bool Handle_S_CONNECT(PacketSessionRef& session, Protocol::S_CONNECT& pkt);
bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt);

class ServerPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[PKT_S_TEST] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_TEST>(Handle_S_TEST, session, buffer, len); };
		GPacketHandler[PKT_S_CONNECT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_CONNECT>(Handle_S_CONNECT, session, buffer, len); };
		GPacketHandler[PKT_S_CHAT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_CHAT>(Handle_S_CHAT, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	static SendBufferRef MakeSendBuffer(Protocol::C_TEST& pkt) { return MakeSendBuffer(pkt, PKT_C_TEST); }
	static SendBufferRef MakeSendBuffer(Protocol::C_CONNECT& pkt) { return MakeSendBuffer(pkt, PKT_C_CONNECT); }
	static SendBufferRef MakeSendBuffer(Protocol::C_CHAT& pkt) { return MakeSendBuffer(pkt, PKT_C_CHAT); }

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