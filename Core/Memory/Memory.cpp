#include "pch.h"
#include "Memory.h"
#include "MemoryPool.h"

Memory::Memory()
{
	int32 size = 0;
	int32 tableIndex = 0;
	for (size = 32; size <= 1024; size += 32) {
		MemoryPool* pool = new MemoryPool(size);
		_pools.push_back(pool);

		while (tableIndex <= size) {
			_poolTable[tableIndex] = pool;
			++tableIndex;
		}
	}

	for (; size <= 2048; size += 128) {
		MemoryPool* pool = new MemoryPool(size);
		_pools.push_back(pool);

		while (tableIndex <= size) {
			_poolTable[tableIndex] = pool;
			++tableIndex;
		}
	}

	for (; size <= 4096; size += 256) {
		MemoryPool* pool = new MemoryPool(size);
		_pools.push_back(pool);

		while (tableIndex <= size) {
			_poolTable[tableIndex] = pool;
			++tableIndex;
		}
	}
}

Memory::~Memory()
{
	for (MemoryPool* pool : _pools) {
		delete pool;
	}
	_pools.clear();
}

void* Memory::AlignedAlloc(std::size_t size, std::size_t alignment)
{
#if defined(_WIN32) || defined(_WIN64)
	return ::_aligned_malloc(size, alignment);
#else
	// posix_memalign: alignment는 power-of-two & sizeof(void*) 배수여야 함
	if (alignment < alignof(void*)) {
		alignment = alignof(void*);
	}
	void* p = nullptr;
	int rc = ::posix_memalign(&p, alignment, size);
	return (rc == 0) ? p : nullptr;
#endif
}

void Memory::AlignedFree(void* p)
{
#if defined(_WIN32) || defined(_WIN64)
	::_aligned_free(p);
#else
	::free(p);
#endif

}
void* Memory::Allocate(int32 size)
{
	MemoryHeader* header = nullptr;
	const int32 allocSize = size + sizeof(MemoryHeader);

#ifdef _STOMP
	header = reinterpret_cast<MemoryHeader*>(StompAllocator::Alloc(allocSize));
#else
	if (allocSize > MAX_ALLOC_SIZE) {
		// 메모리 풀링 최대 크기를 벗어나면 일반 항당.
		header = reinterpret_cast<MemoryHeader*>(AlignedAlloc(allocSize, SLIST_ALIGNMENT));
	} else {
		// 메모리 풀에서 할당.
		header = _poolTable[allocSize]->Pop();
	}
#endif
	return MemoryHeader::AttachHeader(header, allocSize);
}

void Memory::Release(void* ptr)
{
	MemoryHeader* header = MemoryHeader::DetachHeader(ptr);

	const int32 allocSize = header->allocSize;
	ASSERT_CRASH(allocSize > 0);

#ifdef _STOMP
	StompAllocator::Release(header);
#else

	if (allocSize > MAX_ALLOC_SIZE) {
		// 메모리 풀링 최대 크기를 벗어나면 일반 해제
		// ::_aligned_free(header);
		AlignedFree(header);
		// ::free(header);
	} else {
		// 메모리 풀에 반납
		_poolTable[allocSize]->Push(header);
	}
#endif

}
