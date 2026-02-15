# 요약
```
Core 정적 라이브러리 (Core.a)를 만들고,
Protobuf 정적 라이브러리 (libprotobuf.a, libprotobufd.a)를 IMPORTED 타겟으로 붙인 다음. 
빌드가 끝나면 Core 소스의 헤더를 복사 이동
```


# 프로젝트 / 표준 설정
```
cmake_minimum_required(VERSION 3.19)
project(Core LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```
- CMake 최소 버전 3.19
- 프로젝트 이름은 Core
- C++20

# 아키텍쳐 (32/64) 폴더 이름 결정
```
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_DIR "x64")
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(ARCH_DIR "x86")
else()
    set(ARCH_DIR "unknown")
endif()
```
- 포인터 크기로 빌드 아키텍쳐를 판별. ARCH_DIR을 x64 or x86으로 설정
- 이후 라이브러리 경로, 출력 경로에 ARCH_DIR 사용


# 빌드 타입 (Debug/Release) 처리, Protobuf 디렉토리 설정
```
if (NOT CMAKE_CONFIGURATION_TYPES)
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    endif()
    set(PROTO_CONFIG_DIR "${CMAKE_BUILD_TYPE}")
else()
    set(PROTO_CONFIG_DIR "$<CONFIG>")
endif()
```

# Root_Dir 설정
```
set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
```
- 파일이 /home/user/Core/CMakeLists.txt 일시
    - CMAKE_CURRENT_SOURCE_DIR = /home/user/Core
    - ROOT_DIR = /home/user


# Core 소스 파일 목록 정의
```
set(CORE_SOURCES
    ".../CoreGlobal.cpp"
    ...
)
```
- Core 라이브러리에 들어갈 cpp 파일 나열

- [TODO] CMake에서 OS별로 소스 목록 분기 방식 사용


# Core 정적 라이브러리 생성
```
add_library(Core STATIC ${CORE_SOURCES})
```
- 출력물 : libCore.a (리눅스), Core.lib(윈도우)


# Protobuf IMPORTED 타겟으로 연결
```
set(PROTOBUF_INCLUDE_DIR "${ROOT_DIR}/Include/Protobuf")
set(PROTOBUF_LIB_DIR "${ROOT_DIR}/Lib/Protobuf/${PROTO_CONFIG_DIR}/${ARCH_DIR}")
set(PROTOBUF_LIB_PATH "${PROTOBUF_LIB_DIR}/libprotobuf.a")
if (EXISTS "${PROTOBUF_LIB_DIR}/libprotobufd.a")
    set(PROTOBUF_LIB_PATH "${PROTOBUF_LIB_DIR}/libprotobufd.a")
endif()
```
- 헤더 : /home/user/Include/Protobuf
- 라이브러리 : /home/user/Lib/Protobuf/<Debug | Release>/<x64 | x86>
- 기본은 libprotobuf.a
- libprotobufd.a 가 있으면 그걸 사용
- [TODO] 변경 Debug 라이브러리를 자동으로 우선 선택

# Protobuf IMPORTED 타겟 생성
```
add_library(Protobuf::libprotobuf STATIC IMPORTED GLOBAL)
set_target_properties(Protobuf::libprotobuf PROPERTIES
    IMPORTED_LOCATION "${PROTOBUF_LIB_PATH}"
    INTERFACE_INCLUDE_DIRECTORIES "${PROTOBUF_INCLUDE_DIR}"
)
target_link_libraries(Core PUBLIC Protobuf::libprotobuf)
```
- Protobuf::libprotobuf는 CMake가 빌드하지 않는 외부 라이브러리.
- IMPORTED_LOCATION에 실제 파일 경로를 지정
- INTERFACE_INCLUDE_DIRECTORIES로 include 경로를 걸어둘시,
    - Core가 PUBLIC으로 링크했기 때문에, Core를 사용하는 타겟들도 protobuf include 자동 전파


# Linux에서 필요한 시스템 라이브러리 추가
```
if (UNIX AND NOT APPLE)
    find_package(Threads REQUIRED)
    target_link_libraries(Core PUBLIC Threads::Threads dl)
endif()
```
- pthread (Threads::Threads) 링크
- dl (dynamic loader) 링크


# include 디렉토리 Public
```
target_include_directories(Core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core"
    ...
)
```
- Core 내부가 포함해야 하는 헤더 경로 + Core를 사용하는 쪽도 봐야하는 헤더 경로를 public으로 설정
- Core를 링크하는 앱은 [#include "Network/..."] 처럼 바로 사용 가능


# Precompiled Header (PCH) 설정
```
target_precompile_headers(Core PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:pch.h>"
)
```
- Core 타겟을 빌드할 때만 pch.h를 프리컴파일 사용
- $<$<COMPILE_LANGUAGE:CXX>...>는 C++ 컴파일 일때만 적용라는 제너레이터 표현식

# 라이브러리 출력물(.a, .lib) 저장 위치 지정
```
set_target_properties(Core PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../../Lib/Core/${CMAKE_BUILD_TYPE}/${ARCH_DIR}"
)
```
- CMAKE_BINARY_DIR가 /home/user/Core/build 라면
- 결과: /home/user/Lib/Core/<Debug|Release>/<x64|x86>/libCore.a
- multi-config(Visual Studio)에서는 ${CMAKE_BUILD_TYPE}가 비어 있을 수 있어. 윈도우까지 동일 CMake를 쓰려면 여기도 $<CONFIG> 기반으로 분기하는 게 더 안전.


# 헤더 파일 복사
## 헤더 파일 재귀적으로 수집
```
file(GLOB_RECURSE CORE_PUBLIC_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)
```

## 빌드 후 한 폴더로 복사(평탄화)
```
set(CORE_HEADERS_FLAT_DST "${ROOT_DIR}/Include/CoreFlat")

add_custom_command(TARGET Core POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${CORE_HEADERS_FLAT_DST}"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            ${CORE_PUBLIC_HEADERS}
            "${CORE_HEADERS_FLAT_DST}"
    COMMENT "Copy Core headers(flat) -> ${CORE_HEADERS_FLAT_DST}"
    ...
    VERBATIM
)
```