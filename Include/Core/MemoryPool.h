#pragma once
#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include "Types.h"
#include <windows.h>
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>

enum {
	SLIST_ALIGNMENT = 16
};

// [32][64][128][256][512][1024][2048][4096]
// [32 32 32 32 32 32]
DECLSPEC_ALIGN(SLIST_ALIGNMENT)
struct MemoryHeader : public SLIST_ENTRY
{
private:
	// [MemoryHeader][Data]
	MemoryHeader(int32 size) : allocSize(size) {}
public:
	static void* AttachHeader(MemoryHeader* header, int32 size)
	{
		// [MemoryHeader][Data] -> Data 시작 위치로 이동해서 전달.
		new(header)MemoryHeader(size); // placemenet new
		return reinterpret_cast<void*>(++header);
	}

	static MemoryHeader* DetachHeader(void* ptr)
	{
		// [MemoryHeader][Data] -> MemoryHeader 시작 위치로 이동해서 전달.
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}

	int32 allocSize;
};

DECLSPEC_ALIGN(SLIST_ALIGNMENT)
class MemoryPool
{
public:
	MemoryPool(int32 allocSize);
	~MemoryPool();
public:
	void Push(MemoryHeader* ptr);
	MemoryHeader* Pop();
private:
	SLIST_HEADER _header;
	int32 _allocSize = 0;
	Atomic<int32> _useCount = 0;
	Atomic<int32> _reserveCount = 0;
};

#endif // MEMORYPOOL_H