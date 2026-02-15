#pragma once
#ifndef POOLALLOCATOR_H
#define POOLALLOCATOR_H

class PoolAllocator
{
public:
	static void* Alloc(int32 size);
	static void Release(void* ptr);
};

#endif // POOLALLOCATOR_H