#include "pch.h"
#include "Service.h"
#include "IOCPSession.h"
#include "IOCPListener.h"
#include "IOCPCore.h"

#include "EpollSession.h"
#include "EpollListener.h"
#include "EpollCore.h"


Service::Service(ServiceType type, NetAddress address, NetCoreRef core, SessionFactory factory, int32 maxSessionCount)
	: _type(type)
	, _netAddress(address)
	, _core(core)
	, _sessionFactory(factory)
	, _maxSessionCount(maxSessionCount)
{
}

Service::~Service()
{
}

void Service::CloseService()
{
}

void Service::Broadcast(SendBufferRef sendBuffer)
{
	WRITE_LOCK;
	for (const auto& session : _sessions) {
		session->Send(sendBuffer);
	}
}

NetSessionRef Service::CreateSession()
{
	NetSessionRef session = _sessionFactory();
	session->SetService(shared_from_this());

	if (_core->Register(session) == false) {
		return nullptr;
	}
	return session;
}

void Service::AddSession(NetSessionRef session)
{
	WRITE_LOCK;
	_sessionCount++;
	_sessions.insert(session);
}

void Service::ReleaseSession(NetSessionRef session)
{
	WRITE_LOCK;
	ASSERT_CRASH(_sessions.erase(session) != 0);
	_sessionCount--;
}



////////////////////////////////////////////////////////////////////////

ClientService::ClientService(NetAddress targetAddress, NetCoreRef core, SessionFactory factory, int32 maxSessionCount)
	: Service(ServiceType::Client, targetAddress, core, factory, maxSessionCount)
{
}

bool ClientService::Start()
{
	if (CanStart() == false)
		return false;

	const int32 sessionCount = GetMaxSessionCount();
	for (int32 i = 0; i < sessionCount; ++i) {
		NetSessionRef session = CreateSession();
		if (session->Connect() == false)
			return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////

ServerService::ServerService(NetAddress address, NetCoreRef core, SessionFactory factory, int32 maxSessionCount)
	: Service(ServiceType::Server, address, core, factory, maxSessionCount)
{
}

bool ServerService::Start()
{
	if (CanStart() == false) {
		return false;
	}

	_listener = MakeShared<NetListener>();
	if (_listener == nullptr) {
		return false;
	}

	ServerServiceRef service = static_pointer_cast<ServerService>(shared_from_this());
	if (_listener->StartAccept(service) == false) {
		return false;
	}

	return true;
}

void ServerService::CloseService()
{
	Service::CloseService();
}

/*----------------------------------------------------------------------
Service : Start -> Listen -> Accept
Core : Dispatch (µî·ÏµÈ 
			IOCP -> Dispatch  
			Epoll -> Dispatch



*/

