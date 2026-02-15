#pragma once
#ifndef SERVICE_H
#define SERVICE_H

#include "Container.h"
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
	Service(ServiceType type, NetAddress address, NetCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~Service();
public:
	virtual bool Start() = 0;
	virtual void CloseService();
	bool CanStart() { return _sessionFactory != nullptr; }
	void SetSessionFactory(SessionFactory func) { _sessionFactory = func; }

	void Broadcast(SendBufferRef sendBuffer);
	NetSessionRef CreateSession();
	void AddSession(NetSessionRef session);
	void ReleaseSession(NetSessionRef session);
public:
	int32 GetCurrentSessionCount() { return _sessionCount; }
	int32 GetMaxSessionCount() { return _maxSessionCount; }
	ServiceType GetServiceType() { return _type; }
	NetAddress GetNetAddress() { return _netAddress; }
	NetCoreRef& GetNetCore() { return _core; }
protected:
	USE_LOCK;

	ServiceType _type;
	NetAddress _netAddress = {};
	NetCoreRef _core;

	Set<NetSessionRef> _sessions;
	int32 _sessionCount = 0;
	int32 _maxSessionCount = 0;
	SessionFactory _sessionFactory;
	SessionFactoryArg _sessionFactoryArg;
};


////////////////////////////////////////////

class ClientService : public Service
{
public:
	ClientService(NetAddress targetAddress, NetCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~ClientService() {}
public:
	virtual bool Start() override;
};

////////////////////////////////////////////

class ServerService : public Service
{
public:
	ServerService(NetAddress targetAddress, NetCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~ServerService() {}
public:
	virtual bool Start() override;
	virtual void CloseService() override;
private:
	NetListenerRef _listener = nullptr;
};

#endif // SERVICE_H