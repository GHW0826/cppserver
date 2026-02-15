#pragma once
#ifndef EPOLLLISTENER_H
#define EPOLLLISTENER_H

#if defined(__unix__) || defined(__unix) || defined(__linux__)

#include "EpollObject.h"

class EpollListener : public EpollObject
{
public:
	EpollListener() = default;
	~EpollListener();
public:
	bool StartAccept(ServerServiceRef service);
	void CloseSocket();
public:
	virtual int32 GetHandle() override { return _handle; }
	virtual void Dispatch(uint32 epollEvent) override;
private:
	// void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept();
protected:
	int32 _handle;
	ServerServiceRef _service;
};

#endif

#endif //