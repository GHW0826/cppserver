#include "pch.h"

#if defined(__unix__) || defined(__unix) || defined(__linux__)

#include "EpollSession.h"
#include "Service.h"
#include "SocketUtil.h"
#include "Service.h"
#include "EpollCore.h"

EpollSession::EpollSession()
	: _recvBuffer(BUFFER_SIZE)
{
	// _handle = SocketUtil::CreateSocket();
}

EpollSession::~EpollSession()
{
}

void EpollSession::AttachHandle(int32 handle)
{
	_handle = handle;
}


bool EpollSession::Connect()
{
	// cout << "Session::Connect" << endl;
	return RegisterConnect();
}

bool EpollSession::RegisterConnect()
{
	if (IsConnected())
		return false;

	if (GetService()->GetServiceType() != ServiceType::Client)
		return false;

	if (SocketUtil::SetReuseAddress(_handle, true) == false)
		return false;

	if (SocketUtil::BindAnyAddress(_handle, 0) == false)
		return false;

	_service.lock()->AddSession(std::static_pointer_cast<EpollSession>(shared_from_this()));

	SOCKADDR_IN addr = GetService()->GetNetAddress().GetSockAddr();
	if (connect(_handle, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
		std::perror("connect");
		close(_handle);
		return 1;
	}

	_connected.store(true);

	// 콘텐츠 코드에서 오버로딩
	OnConnected();

	return true;
}

void EpollSession::Disconnect(const WideChar* cause)
{
	if (_connected.exchange(false) == false)
		return;
}


void EpollSession::Dispatch(uint32 epollEvent)
{
	if (epollEvent & (EPOLLERR | EPOLLHUP)) {
		// 에러 또는 종료
		std::cout << "Client error/hup, closing fd=" << _handle << std::endl;
		epoll_ctl(GetService()->GetNetCore()->GetHandle(), EPOLL_CTL_DEL, _handle, nullptr);
		close(_handle);
		return;
	}

	if (epollEvent & EPOLLIN) {
		while (true) {
			char* buf = reinterpret_cast<char*>(_recvBuffer.WritePos());
			size_t len = _recvBuffer.FreeSize();
			ssize_t count = read(_handle, buf, len);
			if (count == -1) {
				if (errno == EAGAIN || errno == EWOULDBLOCK) {
					// 더 이상 읽을 데이터 없음
					break;
				} else {
					std::perror("read");
					epoll_ctl(GetService()->GetNetCore()->GetHandle(), EPOLL_CTL_DEL, _handle, nullptr);
					close(_handle);
				}
			} else if (count == 0) {
				// 클라이언트 종료
				std::cout << "Client disconnected fd=" << _handle << std::endl;
				epoll_ctl(GetService()->GetNetCore()->GetHandle(), EPOLL_CTL_DEL, _handle, nullptr);
				close(_handle);
				break;
			}
		}
	}
}

void EpollSession::ProcessConnect()
{
	_connected.store(true);

	// 세션 등록
	GetService()->AddSession(GetSessionRef());

	// 콘텐츠 코드에서 오버로딩
	OnConnected();
}

void EpollSession::ProcessDisconnect()
{
	OnDisconnected(); // 컨텐츠 코드에서 오버로딩
	GetService()->ReleaseSession(GetSessionRef());
}


void EpollSession::ProcessRecv(int32 numOfBytes)
{
	if (numOfBytes == 0) {
		Disconnect("Recv 0)");
		return;
	}
	if (_recvBuffer.OnWrite(numOfBytes) == false) {
		Disconnect("OnWrite Overflow");
		return;
	}

	// 컨텐츠
	int32 dataSize = _recvBuffer.DataSize();
	int32 processLen = OnRecv(_recvBuffer.ReadPos(), dataSize);
	if (processLen < 0 || dataSize < processLen || _recvBuffer.OnRead(processLen) == false) {
		Disconnect("OnRead Overflow");
		return;
	}

	// 커서 정리
	_recvBuffer.Clean();
}



void EpollSession::Send(SendBufferRef sendBuffer)
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
}

#endif