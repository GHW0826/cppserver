#pragma once
#ifndef EPOLLSESSION_H

#if defined(__unix__) || defined(__unix) || defined(__linux__)

#include "EpollObject.h"
#include "Macros.h"
#include "NetAddress.h"
#include "RecvBuffer.h"

class Service;

class EpollSession : public EpollObject
{
	friend class EpollListener;
	friend class EpollCore;
	friend class Service;

	enum {
		BUFFER_SIZE = 0x10000 // 64KB
	};
public:
	EpollSession();
	virtual ~EpollSession();
public:
	// 외부에서 사용
	void Send(SendBufferRef sendBuffer);

	// 클라이언트 서비스용
	bool Connect();
	void Disconnect(const WideChar* cause);
public:
	bool IsConnected() { return _connected; }
	void SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress GetAddress() { return _netAddress; }
	NetHandle GetSocket() { return _handle; }
	NetSessionRef GetSessionRef() { return static_pointer_cast<EpollSession>(shared_from_this()); }
	SharedPtr<Service> GetService() { return _service.lock(); }
	void SetService(SharedPtr<Service> service) { _service = service; }
	void AttachHandle(int32 handle);
protected:
	// 컨텐츠 코드에서 오버로딩
	virtual void OnConnected() {}
	virtual int32 OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void OnSend(int32 len) {}
	virtual void OnDisconnected() {}
private:
	// 전송 관련
	bool RegisterConnect();

	void ProcessConnect();
	void ProcessDisconnect();
	void ProcessRecv(int32 numOfBytes);
	void ProcessSend(int32 numOfBytes);

	void HandleError(int32 errCode);
private:
	// 인터페이스 구현
	virtual NetHandle GetHandle() override { return _handle; }
	virtual void Dispatch(uint32 epollEvent) override;
private:
	WeakedPtr<Service> _service;
	int32 _handle;
	NetAddress _netAddress = {};
	Atomic<bool> _connected = false;
private:
	USE_LOCK;
	// 수신
	RecvBuffer _recvBuffer;
	// 송신
	Queue<SendBufferRef> _sendQueue;
	Atomic<bool> _sendRegistered = false;
};

#endif

#endif