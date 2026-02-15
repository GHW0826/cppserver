#pragma once
#ifndef LISTENER_H
#define LISTENER_H

#if defined(_WIN32) || defined(_WIN64)

#include "IOCPObject.h"

class AcceptEvent;
class ServerService;

class IOCPListener : public IOCPObject
{
public:
	IOCPListener() = default;
	~IOCPListener();
public:
	bool StartAccept(ServerServiceRef service);
	void CloseSocket();
public:
	virtual HANDLE GetHandle() override { return reinterpret_cast<HANDLE>(_socket); }
	virtual void Dispatch(class IOCPEvent* iocpEvent, int32 numOfBytes = 0) override;
private:
	// 수신 관련
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);
protected:
	SOCKET _socket = INVALID_SOCKET;
	Vector<AcceptEvent*> _acceptEvents;
	ServerServiceRef _service;
};
#endif

#endif // LISTENER_H
