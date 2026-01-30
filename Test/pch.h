#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#ifdef _DEBUG
#pragma comment(lib, "Core.lib")
#pragma comment(lib, "x64\\libprotobufd.lib")
#else
#pragma comment(lib, "Release\\Core.lib")
#pragma comment(lib, "x64\\libprotobuf.lib")
#endif

#include "CorePch.h"