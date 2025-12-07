cmake_minimum_required(VERSION 3.19)

project(Server LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 32bit / 64bit Archtecture Name
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_DIR "x64")
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(ARCH_DIR "x86")
else()
    set(ARCH_DIR "unknown")
endif()

# Default Build Type Debug
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Core Lib Sorce
file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Core/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Job/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Network/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Network/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Thread/*.cpp"
)

# Create static Lib
add_library(Core STATIC ${CORE_SOURCES})

# header include path
target_include_directories(Core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Core"          # Core/Core.h 
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/Core"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/Job"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/Network"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/Thread"
)

# ===== Already Compiled Header Set (CMake 3.16+ Recommand) =====
target_precompile_headers(Core PRIVATE
	"$<$<COMPILE_LANGUAGE:CXX>:pch.h>"
)

# ==== Core Lib Output Path Select ===
set_target_properties(Core PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../Lib/Core/${CMAKE_BUILD_TYPE}/${ARCH_DIR}"
)



# Create Executable file Example
# add_executable(ServerApp main.cpp)
# target_link_libraries(ServerApp PRIVATE Core

