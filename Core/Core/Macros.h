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

#endif // !MACROS_H