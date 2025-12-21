#include "pch.h"
#include "ServerSession.h"
#include "ClientPacketHandler.h"

void ServerSession::OnConnected()
{
	// GSessionManager.Add(static_pointer_cast<GameSession>(shared_from_this()));
}

void ServerSession::OnDisconnected()
{
	//GSessionManager.Remove(static_pointer_cast<GameSession>(shared_from_this()));

	//if (_currentPlayer)
	//{
	//	if (auto room = _room.lock())
	//		room->DoAsync(&Room::Leave, _currentPlayer);
	//}

	//_currentPlayer = nullptr;
	//_players.clear();
}

void ServerSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	PacketSessionRef session = GetPacketSessionRef();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	// TODO : packetId 대역 체크
	ClientPacketHandler::HandlePacket(session, buffer, len);
}

void ServerSession::OnSend(int32 len)
{
}