#pragma once
#ifndef SERVICE_H
#define SERVICE_H

#include "Types.h"
#include "NetAddress.h"

enum class ServiceType : uint8
{
	Server,
	Client
};

class Service : public EnableSharedFromThis<Service>
{
public:
	Service(ServiceType type, NetAddress address, NetCoreRef core, SessionFactory<void> factory, int32 maxSessionCount = 1);
	virtual ~Service();
public:
	virtual bool Start() abstract;
	bool CanStart() { return _sessionFactory != nullptr; }
	virtual void CloseService();
	void SetSessionFactory(SessionFactory<void> func) { _sessionFactory = func; }

	void Broadcast(SendBufferRef sendBuffer);
	SessionRef CreateSession();
	void AddSession(SessionRef session);
	void ReleaseSession(SessionRef session);
public:
	int32 GetCurrentSessionCount() { return _sessionCount; }
	int32 GetMaxSessionCount() { return _maxSessionCount; }
	ServiceType GetServiceType() { return _type; }
	NetAddress GetNetAddress() { return _netAddress; }
	NetCoreRef& GetIocpCore() { return _core; }
protected:
	USE_LOCK;

	ServiceType _type;
	NetAddress _netAddress = {};
	NetCoreRef _core;

	Set<SessionRef> _sessions;
	int32 _sessionCount = 0;
	int32 _maxSessionCount = 0;
	SessionFactory<void> _sessionFactory;
};


////////////////////////////////////////////

class ClientService : public Service
{
public:
	ClientService(NetAddress targetAddress, IOCPCoreRef core, SessionFactory<void> factory, int32 maxSessionCount = 1);
	virtual ~ClientService() {}
public:
	virtual bool Start() override;
};

////////////////////////////////////////////

class ServerService : public Service
{
public:
	ServerService(NetAddress targetAddress, IOCPCoreRef core, SessionFactory<void> factory, int32 maxSessionCount = 1);
	virtual ~ServerService() {}
public:
	virtual bool Start() override;
	virtual void CloseService() override;
private:
	ListenerRef _listener = nullptr;
};

#endif // SERVICE_H