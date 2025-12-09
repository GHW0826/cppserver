#include "pch.h"
#include "PoolAllocator.h"


void* PoolAllocator::Alloc(int32 size)
{
	return nullptr;
	// return GMemory->Allocate(size);
}

void PoolAllocator::Release(void* ptr)
{
	// GMemory->Release(ptr);
}