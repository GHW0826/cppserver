#pragma once
#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include "Types.h"
#include "MemoryPool.h"

template<typename Type>
class ObjectPool
{
public:
	template<typename... Args>
	static Type* Pop(Args&&... args)
	{
#if defined(_WIN32) || defined(_WIN64)

#ifdef _STOMP
		MemoryHeader* ptr = reinterpret_cast<MemoryHeader*>(StompAllocator::Alloc(s_allocSize));
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(ptr, s_allocSize));
#else
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(s_pool.Pop(), s_allocSize));
#endif		
		new(memory)Type(std::forward<Args>(args)...); // placement new
		return memory;
#endif

#if defined(__unix__) || defined(__unix) || defined(__linux__)
		return new Type(std::forward<Args>(args)...);
#endif
	}

	static void Push(Type* obj)
	{
#if defined(_WIN32) || defined(_WIN64)
		obj->~Type();
#ifdef _STOMP
		StompAllocator::Release(MemoryHeader::DetachHeader(obj));
#else
		s_pool.Push(MemoryHeader::DetachHeader(obj));
#endif
#endif


#if defined(__unix__) || defined(__unix) || defined(__linux__)
		if (obj != nullptr) {
			delete obj;
		}
#endif
	}
	template<typename... Args>
	static SharedPtr<Type> MakeShared(Args&&... args)
	{
		SharedPtr<Type> ptr = { Pop(std::forward<Args>(args)...), Push };
		return ptr;
	}
private:
	static int32		s_allocSize;
	static MemoryPool	s_pool;
};

template<typename Type>
int32 ObjectPool<Type>::s_allocSize = sizeof(Type) + sizeof(MemoryHeader);

template<typename Type>
MemoryPool ObjectPool<Type>::s_pool{ s_allocSize };

#endif // OBJECTPOOL_H 