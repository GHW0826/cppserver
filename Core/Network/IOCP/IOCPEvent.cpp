#include "pch.h"
#include "IOCPEvent.h"

#if defined(_WIN32) || defined(_WIN64)

IOCPEvent::IOCPEvent(EventType type)
	: _eventType(type)
{
	Init();
}

void IOCPEvent::Init()
{
	OVERLAPPED::hEvent = 0;
	OVERLAPPED::Internal = 0;
	OVERLAPPED::InternalHigh = 0;
	OVERLAPPED::Offset = 0;
	OVERLAPPED::OffsetHigh = 0;
}

/////////////////////////////////////////////////////////////////////

ConnectEvent::ConnectEvent()
	: IOCPEvent(EventType::Connect)
{}

DisconnectEvent::DisconnectEvent()
	: IOCPEvent(EventType::Disconnect)
{}

AcceptEvent::AcceptEvent()
	: IOCPEvent(EventType::Accept)
{}

RecvEvent::RecvEvent()
	: IOCPEvent(EventType::Recv)
{}

SendEvent::SendEvent()
	: IOCPEvent(EventType::Send)
{}

#endif