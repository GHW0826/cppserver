#include "pch.h"

#if defined(_WIN32) || defined(_WIN64)

#include "IOCPSession.h"
#include "SocketUtil.h"
#include "Service.h"
#include "SendBuffer.h"

IOCPSession::IOCPSession()
	: _recvBuffer(BUFFER_SIZE)
{
	_socket = SocketUtil::CreateSocket();
}

IOCPSession::~IOCPSession()
{
	SocketUtil::Close(_socket);
}

void IOCPSession::Send(SendBufferRef sendBuffer)
{
	if (IsConnected() == false) {
		std::cout << "is not connected" << std::endl;
		return;
	}

	// 1) 버퍼 관련
	// 2) sendEvent 관리?, 단일, 여러개, WSASend 중첩.
	bool registerSend = false;
	{
		WRITE_LOCK;

		_sendQueue.push(sendBuffer);
		if (_sendRegistered.exchange(true) == false) {
			registerSend = true;
		}
	}
	
	if (registerSend) {
		// cout << "if register Send" << endl;
		RegisterSend();
	}
}

bool IOCPSession::Connect()
{
	// cout << "Session::Connect" << endl;
	return RegisterConnect();
}

void IOCPSession::Disconnect(const WCHAR* cause)
{
	// cout << "Session::Disconnect" << endl;

	if (_connected.exchange(false) == false)
		return;

	// TEMP
	// wcout << "Disconnect : " << cause << endl;
	RegisterDiconnect();
}

HANDLE IOCPSession::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void IOCPSession::Dispatch(IOCPEvent* iocpEvent, int32 numOfBytes)
{
	switch (iocpEvent->_eventType) {
	case EventType::Connect:
		ProcessConnect();
		break;
	case EventType::Disconnect:
		ProcessDisconnect();
		break;
	case EventType::Recv:
		ProcessRecv(numOfBytes);
		break;
	case EventType::Send:
		ProcessSend(numOfBytes);
		break;
	default:
		break;
	}
}


bool IOCPSession::RegisterConnect()
{
	if (IsConnected())
		return false;

	if (GetService()->GetServiceType() != ServiceType::Client)
		return false;

	if (SocketUtil::SetReuseAddress(_socket, true) == false)
		return false;

	if (SocketUtil::BindAnyAddress(_socket, 0) == false)
		return false;

	_connectEvent.Init();
	_connectEvent._owner = shared_from_this(); // add ref

	DWORD numOfBytes = 0;
	SOCKADDR_IN sockAddr = GetService()->GetNetAddress().GetSockAddr();
	if (false == SocketUtil::ConnectEx(_socket, reinterpret_cast<SOCKADDR*>(&sockAddr), sizeof(sockAddr), nullptr, 0, &numOfBytes, &_connectEvent)) {
		int32 errCode = ::WSAGetLastError();
		if (errCode != WSA_IO_PENDING) {
			std::cout << errCode << std::endl;
			_connectEvent._owner = nullptr; // release ref
			return false;
		}
	}

	return true;
}

bool IOCPSession::RegisterDiconnect()
{
	_disconnectEvent.Init();
	_disconnectEvent._owner = shared_from_this(); // add ref
	if (false == SocketUtil::DisconnectEx(_socket, &_disconnectEvent, TF_REUSE_SOCKET, 0)) {
		int32 errCode = ::WSAGetLastError();
		if (errCode != WSA_IO_PENDING) {
			std::cout << errCode << std::endl;
			_disconnectEvent._owner = nullptr; // release ref
			return false;
		}
	}

	return true;
}

void IOCPSession::RegisterRecv()
{
	if (IsConnected() == false) {
		return;
	}

	_recvEvent.Init();
	_recvEvent._owner = shared_from_this(); // ADD Ref

	DWORD numOfBytes = 0;
	DWORD flags = 0;
	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer.WritePos());
	wsaBuf.len = _recvBuffer.FreeSize();
	if (SOCKET_ERROR == ::WSARecv(_socket, &wsaBuf, 1, OUT & numOfBytes, OUT & flags, &_recvEvent, nullptr)) {
		int32 errCode = ::WSAGetLastError();
		if (errCode != WSA_IO_PENDING) {
			std::cout << errCode << std::endl;
			HandleError(errCode);
			_recvEvent._owner = nullptr; // Release Ref
		}
	}
}

// 한번에 한스레드만 호출
void IOCPSession::RegisterSend()
{
	if (IsConnected() == false) {
		std::cout << "[RegisterSend] is not connected" << std::endl;
		return;
	}

	_sendEvent.Init();
	_sendEvent._owner = shared_from_this(); // ADD Ref
	// 보낼 데이터를 sendEvent에 등록
	{
		WRITE_LOCK;
		int32 writeSize = 0;
		while (_sendQueue.empty() == false) {
			SendBufferRef sendBuffer = _sendQueue.front();

			writeSize += sendBuffer->WriteSize();
			// TODO 예외 체크
			_sendQueue.pop();
			_sendEvent._sendBuffers.push_back(sendBuffer);
		}
	}

	Vector<WSABUF> wsaBufs;
	wsaBufs.reserve(_sendEvent._sendBuffers.size());
	for (SendBufferRef sendBuffer : _sendEvent._sendBuffers) {
		WSABUF wsaBuf;
		wsaBuf.buf = reinterpret_cast<char*>(sendBuffer->Buffer());
		wsaBuf.len = static_cast<LONG>(sendBuffer->WriteSize());
		wsaBufs.push_back(wsaBuf);
	}

	DWORD numOfBytes = 0;
	if (SOCKET_ERROR == ::WSASend(_socket, wsaBufs.data(), static_cast<DWORD>(wsaBufs.size()), OUT & numOfBytes, 0, &_sendEvent, nullptr)) {
		int32 errCode = ::WSAGetLastError();
		if (errCode != WSA_IO_PENDING) {
			std::cout << errCode << std::endl;
			HandleError(errCode);
			_sendEvent._owner = nullptr; // release ref
			_sendEvent._sendBuffers.clear(); // release ref
			_sendRegistered.store(false);
		}
	}
}

void IOCPSession::ProcessConnect()
{
	_connectEvent._owner = nullptr; // release ref
	_connected.store(true);

	// 세션 등록
	GetService()->AddSession(GetSessionRef());

	// 콘텐츠 코드에서 오버로딩
	OnConnected();

	// 수신 등록
	RegisterRecv();
}

void IOCPSession::ProcessDisconnect()
{
	_disconnectEvent._owner = nullptr; // release ref

	OnDisconnected(); // 컨텐츠 코드에서 오버로딩
	GetService()->ReleaseSession(GetSessionRef());
}

void IOCPSession::ProcessRecv(int32 numOfBytes)
{
	_recvEvent._owner = nullptr; // Release Ref

	if (numOfBytes == 0) {
		Disconnect(L"Recv 0)");
		return;
	}

	if (_recvBuffer.OnWrite(numOfBytes) == false) {
		Disconnect(L"OnWrite Overflow");
		return;
	}

	// 컨텐츠
	int32 dataSize = _recvBuffer.DataSize();
	int32 processLen = OnRecv(_recvBuffer.ReadPos(), dataSize);
	if (processLen < 0 || dataSize < processLen || _recvBuffer.OnRead(processLen) == false) {
		Disconnect(L"OnRead Overflow");
		return;
	}

	// 커서 정리
	_recvBuffer.Clean();

	// 수신 등록
	RegisterRecv();
}

void IOCPSession::ProcessSend(int32 numOfBytes)
{
	_sendEvent._owner = nullptr; // release ref
	_sendEvent._sendBuffers.clear();
	if (numOfBytes == 0) {
		Disconnect(L"Send 0");
		return;
	}

	// 컨텐츠 코드에서 오버로딩
	OnSend(numOfBytes);

	WRITE_LOCK;
	if (_sendQueue.empty())
		_sendRegistered.store(false);
	else
		RegisterSend();
}

void IOCPSession::HandleError(int32 errCode)
{
	switch (errCode) {
	case WSAECONNRESET:
	case WSAECONNABORTED:
		Disconnect(L"HandleError");
		break;
	default:
		// TODO Log (Log Thread)
		std::cout << "Handle Error: " << errCode << std::endl;
		break;
	}
}

#endif