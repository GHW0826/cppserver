#pragma once
#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

#if CPP_VERSION >= 11
using BYTE = std::uint8_t;
using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;
using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using Mutex = std::mutex;
#else   // C++11 이전 계열
	#if defined(_WIN32) || defined(_WIN64)
		typedef unsigned char BYTE;
		typedef __int8  int8;
		typedef __int16 int16;
		typedef __int32 int32;
		typedef __int64 int64;
		typedef unsigned __int8  uint8;
		typedef unsigned __int16 uint16;
		typedef unsigned __int32 uint32;
		typedef unsigned __int64 uint64;
	#else // Linux, MacOS
		#include <stdint.h>

		typedef uint8_t BYTE;

		typedef int8_t  int8;
		typedef int16_t int16;
		typedef int32_t int32;
		typedef int64_t int64;
		typedef uint8_t  uint8;
		typedef uint16_t uint16;
		typedef uint32_t uint32;
		typedef uint64_t uint64;
	#endif // !defined(_WIN32) || defined(_WIN64)
#endif // Types




#endif // !TYPES_H