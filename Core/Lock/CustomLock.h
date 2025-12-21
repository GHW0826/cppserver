#pragma once
#ifndef CUSTOMLOCK_H
#define CUSTOMLOCK_H

class CustomLock
{
	enum :uint32 {
		AQUIRE_TIMEOUT_TICK = 10'000,
		MAX_SPIN_COUNT = 5'000,
		WRITE_THREAD_MASK = 0xFFFF'0000,
		READ_COUNT_MASK = 0x0000'FFFF,
		EMPTY_FLAG = 0x0000'0000
	};
public:
	void WriteLock(const char* name);
	void WriteUnlock(const char* name);
	void ReadLock(const char* name);
	void ReadUnlock(const char* name);
private:
	Atomic<uint32> _lockFlag;
	uint16 _writeCount;
};

#endif // CUSTOMLOCK_H
