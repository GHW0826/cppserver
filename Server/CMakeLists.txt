cmake_minimum_required(VERSION 3.19)
project(ServerApp LANGUAGES CXX)

# =========================
# C++ Standard
# =========================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================
# Arch (x64/x86)
# =========================
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_DIR "x64")
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(ARCH_DIR "x86")
else()
    set(ARCH_DIR "unknown")
endif()

# =========================
# Build Type (single-config)
# =========================
if (NOT CMAKE_CONFIGURATION_TYPES)
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    endif()
    set(CONFIG_DIR "${CMAKE_BUILD_TYPE}")
else()
    set(CONFIG_DIR "$<CONFIG>")
endif()

# =========================
# Paths
# =========================
set(ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/..")   # /home/kss/b/server -> /home/kss/b

set(CORE_LIB_DIR "${ROOT_DIR}/Lib/Core/${CONFIG_DIR}/${ARCH_DIR}")
set(PROTO_LIB_DIR "${ROOT_DIR}/Lib/Protobuf/${CONFIG_DIR}/${ARCH_DIR}")

set(CORE_LIB_PATH "${CORE_LIB_DIR}/libCore.a")

# protobuf는 debug에 d가 붙는 경우가 있어 둘 다 대비
set(PROTOBUF_LIB_PATH "${PROTO_LIB_DIR}/libprotobuf.a")
if (EXISTS "${PROTO_LIB_DIR}/libprotobufd.a")
    set(PROTOBUF_LIB_PATH "${PROTO_LIB_DIR}/libprotobufd.a")
endif()

set(CORE_INCLUDE_DIR "${ROOT_DIR}/Core")
set(PROTOBUF_INCLUDE_DIR "${ROOT_DIR}/Include/Protobuf")

# =========================
# Imported: Protobuf
# =========================
add_library(Protobuf::libprotobuf STATIC IMPORTED GLOBAL)
set_target_properties(Protobuf::libprotobuf PROPERTIES
    IMPORTED_LOCATION "${PROTOBUF_LIB_PATH}"
    INTERFACE_INCLUDE_DIRECTORIES "${PROTOBUF_INCLUDE_DIR}"
)

# =========================
# Imported: Core (prebuilt libCore.a)
# =========================
add_library(Core STATIC IMPORTED GLOBAL)
set_target_properties(Core PROPERTIES
    IMPORTED_LOCATION "${CORE_LIB_PATH}"
)

# Core 헤더 전파 (ServerApp이 Core 헤더 include 가능하도록)
target_include_directories(Core INTERFACE
    "${CORE_INCLUDE_DIR}"
    "${CORE_INCLUDE_DIR}/Core"
    "${CORE_INCLUDE_DIR}/Job"
    "${CORE_INCLUDE_DIR}/Lock"
    "${CORE_INCLUDE_DIR}/Memory"
    "${CORE_INCLUDE_DIR}/Memory/Allocator"
    "${CORE_INCLUDE_DIR}/Memory/Buffer"
    "${CORE_INCLUDE_DIR}/Memory/SmartPointer"
    "${CORE_INCLUDE_DIR}/Network"
    "${CORE_INCLUDE_DIR}/Network/Epoll"
    "${CORE_INCLUDE_DIR}/Network/IOCP"
    "${CORE_INCLUDE_DIR}/Protocol"
    "${CORE_INCLUDE_DIR}/Protocol/Protobuf"
    "${CORE_INCLUDE_DIR}/Thread"
)

# ★중요: Core가 protobuf를 쓰므로 "Core의 링크 인터페이스"에 protobuf를 걸어줘야
# ServerApp이 Core만 링크해도 libprotobuf가 최종 링크 라인에 포함된다.
target_link_libraries(Core INTERFACE Protobuf::libprotobuf)

# =========================
# Linux에서 흔히 필요한 시스템 라이브러리
# =========================
if (UNIX AND NOT APPLE)
    find_package(Threads REQUIRED)
    target_link_libraries(Core INTERFACE Threads::Threads dl)

    # protobuf가 zlib을 요구하는 빌드 구성도 흔함. (링크 에러가 inflate/deflate면 필수)
    target_link_libraries(Core INTERFACE z)
endif()

# =========================
# Server executable
# =========================
add_executable(ServerApp
    "${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp"
)

# Core만 링크하면 Core INTERFACE로 protobuf까지 따라오게 구성됨
target_link_libraries(ServerApp PRIVATE Core)

# 실행파일 출력 폴더 (선택)
set_target_properties(ServerApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/bin/${CONFIG_DIR}/${ARCH_DIR}"
)
