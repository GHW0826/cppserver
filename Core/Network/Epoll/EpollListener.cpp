#include "pch.h"

#if defined(__unix__) || defined(__unix) || defined(__linux__)

#include "EpollListener.h"
#include "SocketUtil.h"
#include "Service.h"
#include "EpollCore.h"
#include "EpollSession.h"

EpollListener::~EpollListener()
{
}

bool EpollListener::StartAccept(ServerServiceRef service)
{
	_service = service;
	if (_service == nullptr) {
		return false;
	}

	_handle = SocketUtil::CreateSocket();
	if (_handle == -1) {
		return false;
	}
	if (_service->GetNetCore()->Register(shared_from_this()) == false) {
		return false;
	}
	if (SocketUtil::SetReuseAddress(_handle, true) == false) {
		return false;
	}
	if (SocketUtil::SetLinger(_handle, 0, 0) == false) {
		return false;
	}
	if (SocketUtil::Bind(_handle, _service->GetNetAddress()) == false) {
		return false;
	}
	if (SocketUtil::Listen(_handle) == false) {
		return false;
	}
	if (SocketUtil::SetNonBlocking(_handle) == -1) {
		return false;
	}

	//const int32 acceptCount = _service->GetMaxSessionCount();
	//for (int32 i = 0; i < acceptCount; ++i) {
	//	AcceptEvent* acceptEvent = snew<AcceptEvent>();
	//	acceptEvent->_owner = shared_from_this();
	//	_acceptEvents.push_back(acceptEvent);
	//	RegisterAccept(acceptEvent);
	//}

	return true;
}

void EpollListener::Dispatch(uint32 epollEvent)
{
	ProcessAccept();
}

void EpollListener::ProcessAccept()
{
	while (true) {
		sockaddr_in client_addr{};
		socklen_t client_len = sizeof(client_addr);
		int32 client_fd = accept(_handle, reinterpret_cast<sockaddr*>(&client_addr), &client_len);
		if (client_fd == -1) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				// 더 이상 수락할 것 없음
				break;
			} else {
				std::perror("accept");
				break;
			}
		}
		if (SocketUtil::SetNonBlocking(client_fd) == -1) {
			std::perror("setNonBlocking(client_fd)");
			close(client_fd);
			continue;
		}

		NetSessionRef session = _service->CreateSession(); // Register IOCP;
		if (false == SocketUtil::SetUpdateAcceptSocket(session->GetSocket(), _handle)) {
			return;
		}
		session->AttachSocket(client_fd);

		epoll_event client_ev{};
		client_ev.events = EPOLLIN; // 읽기 이벤트 감지
		client_ev.data.ptr = session.get();
		if (epoll_ctl(_service->GetNetCore()->GetHandle(), EPOLL_CTL_ADD, client_fd, &client_ev) == -1) {
			std::perror("epoll_ctl ADD client_fd");
			close(client_fd);
			continue;
		}

		if (-1 == ::getpeername(session->GetSocket(), OUT reinterpret_cast<SOCKADDR*>(&client_addr), &client_len)) {
			return;
		}

		// cout << "Client Connected!" << endl;
		session->SetNetAddress(NetAddress(client_addr));

		char ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
		std::cout << "New client connected: " << ip << ":" << ntohs(client_addr.sin_port) << " fd=" << client_fd << std::endl;

		session->ProcessConnect();
	}
}




#endif
