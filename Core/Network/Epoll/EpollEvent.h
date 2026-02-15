#pragma once
#ifndef EPOLLEVENT_H
#define EPOLLEVENT_H

#if defined(__unix__) || defined(__unix) || defined(__linux__)

enum class EventType : uint8
{
	Connect,
	Disconnect,
	Accept,
	// PreRecv, 0byte recv
	Recv,
	Send
};

class EpollEvent
{
public:
	EpollEvent(EventType type);
public:
	void Init();
public:
	EventType _eventType;
	NetObjectRef _owner;
};

// ConenctEvent
class ConnectEvent : public EpollEvent
{
public:
	ConnectEvent();
};

// DisconenctEvent
class DisconnectEvent : public EpollEvent
{
public:
	DisconnectEvent();
};

// AcceptEvent
class AcceptEvent : public EpollEvent
{
public:
	AcceptEvent();
public:
	NetSessionRef _session = nullptr;
};


// RecvEvent
class RecvEvent : public EpollEvent
{
public:
	RecvEvent();
};

// SendEvent
class SendEvent : public EpollEvent
{
public:
	SendEvent();
	Vector<SendBufferRef> _sendBuffers;
};

#endif

#endif