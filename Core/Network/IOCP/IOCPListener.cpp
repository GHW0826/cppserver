#include "pch.h"
#include "IOCPListener.h"
#include "SocketUtil.h"
#include "Service.h"
#include "Memory.h"

#if defined(_WIN32) || defined(_WIN64)

#include "IOCPSession.h"
#include "IOCPCore.h"

IOCPListener::~IOCPListener()
{
	SocketUtil::Close(_socket);

	for (AcceptEvent* acceptEvent : _acceptEvents) {
		sdelete<AcceptEvent>(acceptEvent);
	}
}

bool IOCPListener::StartAccept(ServerServiceRef service)
{
	_service = service;
	if (_service == nullptr)
		return false;

	_socket = SocketUtil::CreateSocket();
	if (_socket == INVALID_SOCKET)
		return false;

	if (_service->GetNetCore()->Register(shared_from_this()) == false)
		return false;

	if (SocketUtil::SetReuseAddress(_socket, true) == false)
		return false;

	if (SocketUtil::SetLinger(_socket, 0, 0) == false)
		return false;

	if (SocketUtil::Bind(_socket, _service->GetNetAddress()) == false)
		return false;

	if (SocketUtil::Listen(_socket) == false)
		return false;

	const int32 acceptCount = _service->GetMaxSessionCount();
	for (int32 i = 0; i < acceptCount; ++i) {
		AcceptEvent* acceptEvent = snew<AcceptEvent>();
		acceptEvent->_owner = shared_from_this();
		_acceptEvents.push_back(acceptEvent);
		RegisterAccept(acceptEvent);
	}

	return true;
}

void IOCPListener::CloseSocket()
{
	SocketUtil::Close(_socket);
}


void IOCPListener::Dispatch(IOCPEvent* iocpEvent, int32 numOfBytes)
{
	ASSERT_CRASH(iocpEvent->_eventType == EventType::Accept);
	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(iocpEvent);
	ProcessAccept(acceptEvent);
}

void IOCPListener::RegisterAccept(AcceptEvent* acceptEvent)
{
	IOCPSessionRef session = _service->CreateSession(); // Register IOCP;

	acceptEvent->Init();
	acceptEvent->_session = session;

	DWORD bytesReceived = 0;
	if (false == SocketUtil::AcceptEx(_socket, session->GetSocket(),
		session->_recvBuffer.WritePos(), 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
		OUT & bytesReceived, static_cast<LPOVERLAPPED>(acceptEvent))) {

		const int32 errCode = WSAGetLastError();
		if (errCode != WSA_IO_PENDING) {
			std::cout << errCode << std::endl;
			// 일단 다시Accept 걸어줌
			RegisterAccept(acceptEvent);
		}
	}
}

void IOCPListener::ProcessAccept(AcceptEvent* acceptEvent)
{
	IOCPSessionRef session = acceptEvent->_session;
	if (false == SocketUtil::SetUpdateAcceptSocket(session->GetSocket(), _socket)) {
		RegisterAccept(acceptEvent);
		return;
	}

	SOCKADDR_IN sockAddress;
	int32 sizeOfSockAddr = sizeof(sockAddress);
	if (SOCKET_ERROR == ::getpeername(session->GetSocket(), OUT reinterpret_cast<SOCKADDR*>(&sockAddress), &sizeOfSockAddr)) {
		RegisterAccept(acceptEvent);
		return;
	}

	// cout << "Client Connected!" << endl;
	session->SetNetAddress(NetAddress(sockAddress));
	session->ProcessConnect();
	RegisterAccept(acceptEvent);
}

#endif
