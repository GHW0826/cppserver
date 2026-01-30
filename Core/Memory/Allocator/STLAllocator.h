#pragma once
#ifndef STLALLOCATOR_H
#define STLALLOCATOR_H

#include "PoolAllocator.h"

template<typename T>
class STLAllocator
{
public:
	using value_type = T;

	STLAllocator() {}

	template<typename Other>
	STLAllocator(const STLAllocator<Other>&) {}

	T* allocate(size_t count)
	{
		const int32 size = static_cast<int32>(count * sizeof(T));
		return static_cast<T*>(PoolAllocator::Alloc(size));
		// return static_cast<T*>(salloc(size));
	}

	void deallocate(T* ptr, size_t count)
	{
		PoolAllocator::Release(ptr);
		// srelease(ptr);
	}


	template<typename U>
	bool operator==(const STLAllocator<U>&) { return true; }
	template<typename U>
	bool operator!=(const STLAllocator<U>&) { return false; }
};

#endif // STLALLOCATOR_H