#pragma once
#ifndef CUSTOMWRITELOCKGUARD_H
#define CUSTOMWRITELOCKGUARD_H

#include "CustomLock.h"

class CustomWriteLockGuard
{
public:
	CustomWriteLockGuard(CustomLock& lock, const char* name)
		: _lock(lock), _name(name)
	{
		_lock.WriteLock(name);
	}
	~CustomWriteLockGuard()
	{
		_lock.WriteUnlock(_name);
	}
private:
	CustomLock& _lock;
	const char* _name;
};

#endif // CUSTOMWRITELOCKGUARD_H
