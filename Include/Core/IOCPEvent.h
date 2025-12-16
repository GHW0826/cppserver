#pragma once
#ifndef IOCPEVENT_H
#define IOCPEVENT_H

#if defined(_WIN32) || defined(_WIN64)

#include "Types.h"

enum class EventType : uint8
{
	Connect,
	Disconnect,
	Accept,
	// PreRecv, 0byte recv
	Recv,
	Send
};

// 가상함수 사용 x
// 가상함수 테이블이 맨앞에 위치해,
// OVERLAPPED 포인터 원래 위치의 데이터가 오염될 수 있음.
// OVERLAPPED EX 역할
class IOCPEvent : public OVERLAPPED
{
public:
	IOCPEvent(EventType type);
public:
	void Init();
public:
	EventType _eventType;
	IOCPObjectRef _owner;
};

// ConenctEvent
class ConnectEvent : public IOCPEvent
{
public:
	ConnectEvent();
};

// DisconenctEvent
class DisconnectEvent : public IOCPEvent
{
public:
	DisconnectEvent();
};

// AcceptEvent
class AcceptEvent : public IOCPEvent
{
public:
	AcceptEvent();
public:
	SessionRef _session = nullptr;
};


// RecvEvent
class RecvEvent : public IOCPEvent
{
public:
	RecvEvent();
};

// SendEvent
class SendEvent : public IOCPEvent
{
public:
	SendEvent();

	Vector<SendBufferRef> _sendBuffers;
};

#endif // win

#endif // IOCPEVENT_H