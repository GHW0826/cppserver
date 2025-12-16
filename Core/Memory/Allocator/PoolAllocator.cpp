#include "pch.h"
#include "PoolAllocator.h"

void* PoolAllocator::Alloc(int32 size)
{
    if (size <= 0)
        size = 1;
    return ::operator new(static_cast<size_t>(size));  // 실패 시 std::bad_alloc 발생
	// return nullptr;
	// return GMemory->Allocate(size);
}

void PoolAllocator::Release(void* ptr)
{
    ::operator delete(ptr); // ptr == nullptr 이어도 안전
	// GMemory->Release(ptr);
}