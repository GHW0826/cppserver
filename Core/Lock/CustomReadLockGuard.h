#pragma once
#ifndef CUSTOMREADLOCKGUARD_H
#define CUSTOMREADLOCKGUARD_H

#include "CustomLock.h"

class CustomReadLockGuard
{
public:
	CustomReadLockGuard(CustomLock& lock, const char* name)
		: _lock(lock), _name(name)
	{
		_lock.ReadLock(name);
	}
	~CustomReadLockGuard()
	{
		_lock.ReadUnlock(_name);
	}
private:
	CustomLock& _lock;
	const char* _name;
};

#endif // CUSTOMREADLOCKGUARD_H