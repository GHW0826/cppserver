#pragma once
#ifndef SESSION_H
#define SESSION_H


#if defined(_WIN32) || defined(_WIN64)

#include "Macros.h"
#include "NetAddress.h"
#include "RecvBuffer.h"
#include "IOCPEvent.h"
#include "IOCPObject.h"

class Service;

class IOCPSession : public IOCPObject
{
	friend class IOCPListener;
	friend class IOCPCore;
	friend class Service;

	enum {
		BUFFER_SIZE = 0x10000 // 64KB
	};
public:
	IOCPSession();
	virtual ~IOCPSession();
public:
	// 외부에서 사용
	void Send(SendBufferRef sendBuffer);

	// 클라이언트 서비스용
	bool Connect();
	void Disconnect(const WCHAR* cause);
public:
	bool IsConnected() { return _connected; }
	void SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress GetAddress() { return _netAddress; }
	SOCKET GetSocket() { return _socket; }
	IOCPSessionRef GetSessionRef() { return static_pointer_cast<IOCPSession>(shared_from_this()); }
	SharedPtr<Service> GetService() { return _service.lock(); }
	void SetService(SharedPtr<Service> service) { _service = service; }
protected:
	// 컨텐츠 코드에서 오버로딩
	virtual void OnConnected() {}
	virtual int32 OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void OnSend(int32 len) {}
	virtual void OnDisconnected() {}
private:
	// 전송 관련
	bool RegisterConnect();
	bool RegisterDiconnect();
	void RegisterRecv();
	void RegisterSend();

	void ProcessConnect();
	void ProcessDisconnect();
	void ProcessRecv(int32 numOfBytes);
	void ProcessSend(int32 numOfBytes);

	void HandleError(int32 errCode);
private:
	// 인터페이스 구현
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IOCPEvent* iocpEvent, int32 numOfBytes = 0) override;
private:
	WeakedPtr<Service> _service;
	SOCKET _socket = INVALID_SOCKET;
	NetAddress _netAddress = {};
	Atomic<bool> _connected = false;
private:
	USE_LOCK;
	// 수신
	RecvBuffer _recvBuffer;
	// 송신
	Queue<SendBufferRef> _sendQueue;
	Atomic<bool> _sendRegistered = false;
private:
	RecvEvent _recvEvent;
	SendEvent _sendEvent;
	ConnectEvent _connectEvent;
	DisconnectEvent _disconnectEvent;
};

#endif

#endif // SESSION_H