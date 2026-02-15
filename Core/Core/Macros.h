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
#define CPP_VERSION 03  // 보통 C++98을 legacy C++로 봄. 199711L
#endif


#define TLS thread_local

// thread_local
#if CPP_VERSION >= 11
#elif defined(_MSC_VER) // MSVC: __declspec(thread)
#define TLS __declspec(thread)
#elif defined(__GNUC__) // GCC/Clang: __thread
#define TLS __thread
#else
#error "No thread-local storage keyword available on this compiler."
#endif

// ===== Analysis assume (compiler hint) =====
#if defined(_MSC_VER)
#define ANALYSIS_ASSUME(x) __analysis_assume(x)
#elif defined(__clang__)
#define ANALYSIS_ASSUME(x) __builtin_assume(x)
#elif defined(__GNUC__)
	// GCC에는 __builtin_assume이 없어서 unreachable로 대체
#define ANALYSIS_ASSUME(x) do { if(!(x)) __builtin_unreachable(); } while(0)
#else
#define ANALYSIS_ASSUME(x) ((void)0)
#endif


#define USE_MANY_LOCK(count)	CustomLock _locks[count];
#define USE_LOCK				USE_MANY_LOCK(1)
#define READ_LOCK_IDX(idx)		CustomReadLockGuard readLockGuard_##idx(_locks[idx], typeid(this).name());
#define READ_LOCK				READ_LOCK_IDX(0)
#define WRITE_LOCK_IDX(idx)		CustomWriteLockGuard writeLockGuard_##idx(_locks[idx], typeid(this).name());
#define WRITE_LOCK				WRITE_LOCK_IDX(0)

#if defined(_WIN32) || defined(_WIN64)

// Crash
#define CRASH(cause)						\
{											\
	uint32* crash = nullptr;				\
	ANALYSIS_ASSUME(crash != nullptr);		\
	*crash = 0xDEADBEEF;					\
}

#define ASSERT_CRASH(expr)					\
{											\
	if (!(expr)) {							\
		CRASH("ASSERT_CRASH");				\
		ANALYSIS_ASSUME(expr);				\
	}										\
}

#endif

#if defined(__unix__) || defined(__unix) || defined(__linux__)

// ===== Linux/Unix =====
#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#if defined(__GNUC__) || defined(__clang__)
#define TRAP() __builtin_trap()
#else
#define TRAP() abort()
#endif

#define CRASH(cause)                                                        \
    do {                                                                    \
        fprintf(stderr, "[CRASH] %s | %s:%d (%s)\n",                        \
                (cause), __FILE__, __LINE__, __func__);                     \
        fflush(stderr);                                                     \
        TRAP();                                                             \
    } while(0)

#define ASSERT_CRASH(expr)                                                  \
	if (!(expr)) {															\
		CRASH("ASSERT_CRASH");												\
		ANALYSIS_ASSUME(expr);												\
	}																		\

#endif



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

#define size16(val) static_cast<int16>(sizeof(val))
#define size32(val) static_cast<int32>(sizeof(val))
#define len16(arr) static_cast<int16>(sizeof(arr) / sizeof(arr[0]))
#define len32(arr) static_cast<int32>(sizeof(arr) / sizeof(arr[0]))

#endif // !MACROS_H