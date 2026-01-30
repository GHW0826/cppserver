#pragma once
#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include "Types.h"
#if defined(_WIN32) || defined(_WIN64)

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
#else

#define SLIST_ALIGNMENT 16

struct alignas(SLIST_ALIGNMENT) MemoryHeader
{
    // [2] SLIST_ENTRY 대체 -> 직접 next 포인터 관리
    MemoryHeader* next = nullptr;
    int32 allocSize;

private:
    MemoryHeader(int32 size) : allocSize(size), next(nullptr) {}

public:
    static void* AttachHeader(MemoryHeader* header, int32 size)
    {
        // [MemoryHeader][Data] -> Data 시작 위치로 이동해서 반환
        new(header)MemoryHeader(size); // placement new

        // 포인터 연산: header + 1 은 sizeof(MemoryHeader)만큼 뒤로 이동함
        return reinterpret_cast<void*>(header + 1);
    }

    static MemoryHeader* DetachHeader(void* ptr)
    {
        // [MemoryHeader][Data] -> MemoryHeader 시작 위치로 이동해서 반환
        // 포인터 연산: ptr을 MemoryHeader*로 변환 후 -1 하면 sizeof(MemoryHeader)만큼 앞으로 이동
        MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
        return header;
    }
};

class MemoryPool
{
public:
    MemoryPool(int32 allocSize) : _allocSize(allocSize)
    {
        _head = nullptr;
    }

    ~MemoryPool()
    {
        // 메모리 해제 로직이 필요하다면 여기에 작성
        while (MemoryHeader* memory = Pop())
        {
            // ::operator delete(memory); // 예시: 할당 방식에 따라 해제
        }
    }

public:
    void Push(MemoryHeader* ptr)
    {
        // [3] Lock-Free SLIST 대체 -> Mutex를 이용한 스택 동작 구현
        std::lock_guard<std::mutex> lock(_mutex);

        ptr->next = _head;
        _head = ptr;

        _useCount.fetch_sub(1);
        _reserveCount.fetch_add(1);
    }

    MemoryHeader* Pop()
    {
        std::lock_guard<std::mutex> lock(_mutex);

        if (_head == nullptr)
            return nullptr;

        MemoryHeader* memory = _head;
        _head = memory->next;

        _useCount.fetch_add(1);
        _reserveCount.fetch_sub(1);

        return memory;
    }

private:
    // [4] SLIST_HEADER 대체 -> 일반 포인터와 Mutex 조합
    MemoryHeader* _head = nullptr;
    std::mutex _mutex;

    int32 _allocSize = 0;
    std::atomic<int32> _useCount{ 0 };
    std::atomic<int32> _reserveCount{ 0 };
};

#endif // win


#endif // MEMORYPOOL_H