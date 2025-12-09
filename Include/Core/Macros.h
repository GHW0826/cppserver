#pragma once
#ifndef MACROS_H
#define MACROS_H

#define OUT

// C++ Version
#if __cplusplus >= 202002L
#define CPP_VERSION 20
#elif __cplusplus >= 201703L
#define CPP_VERSION 17
#elif __cplusplus >= 201402L
#define CPP_VERSION 14
#elif __cplusplus >= 201103L
#define CPP_VERSION 11
#else
#define CPP_VERSION 03  // º¸Åë C++98À» legacy C++·Î º½. 199711L
#endif


// thread_local
#if CPP_VERSION >= 11
#define TLS thread_local
#elif defined(_MSC_VER) // MSVC: __declspec(thread)
#define TLS __declspec(thread)
#elif defined(__GNUC__) // GCC/Clang: __thread
#define TLS __thread
#else
#error "No thread-local storage keyword available on this compiler."
#endif


// Crash
#define CRASH(cause)						\
{											\
	uint32* crash = nullptr;				\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xDEADBEEF;					\
}

#define ASSERT_CRASH(expr)					\
{											\
	if (!(expr)) {							\
		CRASH("ASSERT_CRASH");				\
		__analysis_assume(expr);			\
	}										\
}


// Pointer Alloc
#define RAW_POINTER_NEW(name, type)		\
{										\
	if (name == nullptr)				\
		name = new type;				\
}										\

#define RAW_POINTER_DELETE(name)		\
{										\
	if (name != nullptr) {				\
		delete name;					\
		name = nullptr;					\
	}									\
}										\

#define USING_SHARED_PTR(name)	using name##Ref = SharedPtr<class name>;


#define USE_MANY_LOCK(count)	CustomLock _locks[count];
#define USE_LOCK				USE_MANY_LOCK(1);
#define READ_LOCK_IDX(idx)		CustomReadLockGuard readLockGuard_##idx(_locks[idx], typeid(this).name());
#define READ_LOCK				READ_LOCK_IDX(0);
#define WRITE_LOCK_IDX(idx)		CustomWriteLockGuard writeLockGuard_##idx(_locks[idx], typeid(this).name());
#define WRITE_LOCK				WRITE_LOCK_IDX(0);



#define size16(val) static_cast<int16>(sizeof(val))
#define size32(val) static_cast<int32>(sizeof(val))
#define len16(arr) static_cast<int16>(sizeof(arr) / sizeof(arr[0]))
#define len32(arr) static_cast<int32>(sizeof(arr) / sizeof(arr[0]))

#endif // !MACROS_H