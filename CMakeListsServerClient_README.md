# 요약
```
ServerApp 실행 파일을 빌드

미리 빌드된 정적 라이브러리인
Core (libCore.a / Core.lib)
Protobuf (libprotobuf.a / libprotobufd.a)
를 IMPORTED 타겟으로 붙임.

Debug/Release 및 x64/x86 폴더 구조에 맞춰 자동으로 올바른 .a/.lib 경로를 선택.
Core::Core가 Protobuf::libprotobuf를 INTERFACE 의존성으로 들고 있게 해서,
ServerApp은 Core만 링크해도 protobuf까지 자동으로 따라오게 만든다.
Linux에서는 필요한 시스템 라이브러리(Threads, dl, ZLIB)를 조건부로 추가.
PCH(pch.h) 적용, 실행 파일 출력 디렉토리 통일.
```


# 프로젝트 / 표준 설정
```
cmake_minimum_required(VERSION 3.19)
project(ServerApp LANGUAGES CXX)
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
if (CMAKE_CONFIGURATION_TYPES)
    set(IS_MULTI_CONFIG TRUE)
else()
    set(IS_MULTI_CONFIG FALSE)
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    endif()
endif()
```
- Visual Studio 같은 경우는 multi-config( Debug/Release가 한 프로젝트에 공존 )
- Make/Ninja single-config는 CMAKE_BUILD_TYPE 하나만 존재.s 없으면 기본 Debug로 설정

# 경로/이름 설정
```
set(ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/..")

set(CORE_LIB_ROOT   "${ROOT_DIR}/Lib/Core")
set(PROTO_LIB_ROOT  "${ROOT_DIR}/Lib/Protobuf")

set(CORE_LIB_NAME "libCore.a")
set(PROTO_LIB_NAME_RELEASE "libprotobuf.a")
set(PROTO_LIB_NAME_DEBUG   "libprotobufd.a")  # 없을 수도 있음

set(CORE_INCLUDE_DIR     "${ROOT_DIR}/Include/Core")
set(PROTOBUF_INCLUDE_DIR "${ROOT_DIR}/Include/Protobuf")
```
- ROOT_DIR는 상위 폴더(프로젝트 루트)를 기준으로 잡음
- Core/Protobuf 정적 라이브러리 루트 경로 지정
- protobuf는 Debug용(libprotobufd.a)이 존재할 수도/없을 수도 있어서 분기 대상
- include 디렉토리는 Include/Core, Include/Protobuf 기준


# 헬퍼 함수: IMPORTED 타겟의 “config별 라이브러리 경로” 설정
```
function(set_imported_static_locations target lib_root lib_name_debug lib_name_release)
    ...
endfunction()
```
- add_library(... IMPORTED)로 만든 외부 정적 라이브러리에 대해
- Debug/Release/RelWithDebInfo/MinSizeRel에 맞는 실제 파일 경로를 자동 지정
- multi-config일 때
    - IMPORTED_LOCATION_DEBUG, IMPORTED_LOCATION_RELEASE 같은 config별 속성을 지정
RelWithDebInfo, MinSizeRel은 Release로 매핑
- single-config일 때
    - 현재 CMAKE_BUILD_TYPE에 해당하는 폴더에서 .a 경로를 선택
- protobuf Debug 우선 선택 로직
- Debug일 때 libprotobufd.a가 있으면 그걸 쓰고,
- 없으면 libprotobuf.a로 fallback

# Protobuf IMPORTED 타겟 생성 + include 전파
```
add_library(Protobuf::libprotobuf STATIC IMPORTED GLOBAL)

set_imported_static_locations(
    Protobuf::libprotobuf
    "${PROTO_LIB_ROOT}"
    "${PROTO_LIB_NAME_DEBUG}"
    "${PROTO_LIB_NAME_RELEASE}"
)

set_target_properties(Protobuf::libprotobuf PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${PROTOBUF_INCLUDE_DIR}"
)
```
- Protobuf는 외부에서 빌드되어 이미 존재하는 정적 라이브러리
- INTERFACE_INCLUDE_DIRECTORIES를 걸어두면
    - 이 타겟을 링크하는 타겟은 protobuf include 경로를 자동으로 얻게 됨


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

# Core IMPORTED 타겟 생성 + include 전파
```
add_library(Core::Core STATIC IMPORTED GLOBAL)
set_imported_static_locations(
    Core::Core
    "${CORE_LIB_ROOT}"
    "${CORE_LIB_NAME}"
    "${CORE_LIB_NAME}"
)
set_target_properties(Core::Core PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CORE_INCLUDE_DIR}"
)
```
- Core도 이미 빌드된 정적 라이브러리로 전제
- Core 헤더는 너가 Core 빌드 후 ROOT_DIR/Include/Core로 복사해둔 것을 사용

# Core가 Protobuf 의존성을 전파하도록 설정
```
target_link_libraries(Core::Core INTERFACE Protobuf::libprotobuf)
```
- Core를 쓰는 애는 protobuf도 필요하다를 Core 타겟에 명시해둠
- 결과적으로 ServerApp이 Core::Core만 링크해도 protobuf가 자동 링크됨
- 정적 링크 환경에서는 최종 exe 단계에서 protobuf 심볼 해소가 필요하기 때문에
- 이런 식으로 의존성 전파를 해두는 게 깔끔함.


# Linux 시스템 라이브러리 조건부 링크
```
if (UNIX AND NOT APPLE)
    find_package(Threads REQUIRED)
    target_link_libraries(Core::Core INTERFACE Threads::Threads dl)

    find_package(ZLIB)
    if (ZLIB_FOUND)
        target_link_libraries(Core::Core INTERFACE ZLIB::ZLIB)
    else()
        # 필요 시: target_link_libraries(Core::Core INTERFACE z)
    endif()
endif()
```
- Linux에서 흔히 필요한 것들:
- pthread → Threads::Threads
- dl → dlopen 등 동적 로더
- protobuf static을 쓰면 환경에 따라 zlib가 필요할 수 있음
    - 있으면 ZLIB::ZLIB로 링크
    - 없으면 -lz로 해결되는 경우도 있으니 주석 안내


# ServerApp 실행 파일 생성
```
add_executable(ServerApp
    ...
)
```
- 실제 빌드에서는 여기에 cpp 파일 목록이 들어가게 됨


# ServerApp include 디렉토리 설정
```
target_include_directories(ServerApp PRIVATE
    "${PROTOBUF_INCLUDE_DIR}"
    ...
)
```
- ServerApp 내부에서 필요한 include 경로들 추가
- protobuf include는 여기서도 넣지만,
- 사실 Core::Core가 protobuf 의존을 전파하므로, 상황에 따라 중복이 될 수도 있음
- (그래도 직접 protobuf 헤더를 쓰는 소스가 있으면 여기 남겨두는 게 안전)


# PCH 설정
```
target_precompile_headers(ServerApp PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/pch.h>"
)
```
- ServerApp 소스에서 pch.h를 프리컴파일로 사용
- C++ 컴파일일 때만 적용


# 링크 구성
```
target_link_libraries(ServerApp PRIVATE Core::Core)
```
- ServerApp은 Core만 링크한다.
- Core가 INTERFACE로 protobuf, Threads, dl, zlib 등을 전파하므로
- ServerApp은 의존성을 직접 다 몰라도 됨.

# 실행 파일 출력 디렉토리 설정
```
set(OUT_BASE "${CMAKE_BINARY_DIR}/out/bin")

if (IS_MULTI_CONFIG)
    set_target_properties(ServerApp PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${OUT_BASE}/Debug/${ARCH_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${OUT_BASE}/Release/${ARCH_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${OUT_BASE}/RelWithDebInfo/${ARCH_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${OUT_BASE}/MinSizeRel/${ARCH_DIR}"
    )
else()
    set_target_properties(ServerApp PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUT_BASE}/${CMAKE_BUILD_TYPE}/${ARCH_DIR}"
    )
endif()
```
- 빌드 산출물을 build/out/bin/<Config>/<Arch>/ServerApp로 정리
- multi-config는 config별로 따로 설정
- single-config는 ${CMAKE_BUILD_TYPE} 기반