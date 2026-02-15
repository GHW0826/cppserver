cmake_minimum_required(VERSION 3.19)
project(Core LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_DIR "x64")
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(ARCH_DIR "x86")
else()
    set(ARCH_DIR "unknown")
endif()


if (NOT CMAKE_CONFIGURATION_TYPES)
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    endif()
    set(PROTO_CONFIG_DIR "${CMAKE_BUILD_TYPE}")
else()
    set(PROTO_CONFIG_DIR "$<CONFIG>")
endif()

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")

set(CORE_SOURCES
"${CMAKE_CURRENT_SOURCE_DIR}/Job/GlobalQueue.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Job/Job.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Job/JobQueue.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Job/JobTimer.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Lock/CustomLock.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Lock/CustomReadLockGuard.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Lock/CustomWriteLockGuard.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Lock/DeadLockProfiler.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory/Allocator/PoolAllocator.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory/Allocator/STLAllocator.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory/Buffer/RecvBuffer.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory/Buffer/SendBuffer.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory/Buffer/SendBufferChunk.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory/Buffer/SendBufferManager.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory/Memory.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory/MemoryPool.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory/SmartPointer/CustomEnableSharedFromThis.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/Epoll/EpollCore.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/Epoll/EpollEvent.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/Epoll/EpollListener.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/Epoll/EpollObject.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/Epoll/EpollSession.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/IOCP/IOCPCore.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/IOCP/IOCPEvent.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/IOCP/IOCPListener.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/IOCP/IOCPObject.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/IOCP/IOCPSession.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/NetAddress.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/PacketSession.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/Service.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/SocketUtil.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/SocketUtil_posix.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/SocketUtil_win.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Protocol/Protobuf/Enum.pb.cc"
"${CMAKE_CURRENT_SOURCE_DIR}/Protocol/Protobuf/Protocol.pb.cc"
"${CMAKE_CURRENT_SOURCE_DIR}/Protocol/Protobuf/Struct.pb.cc"
"${CMAKE_CURRENT_SOURCE_DIR}/ProtocolPacketHandler.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Thread/ThreadManager.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}Global.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}Pch.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}TLS.cpp"

)


add_library(Core STATIC ${CORE_SOURCES})



set(PROTOBUF_INCLUDE_DIR "${ROOT_DIR}/Include/Protobuf")
set(PROTOBUF_LIB_DIR "${ROOT_DIR}/Lib/Protobuf/${PROTO_CONFIG_DIR}/${ARCH_DIR}")
set(PROTOBUF_LIB_PATH "${PROTOBUF_LIB_DIR}/libprotobuf.a")
if (EXISTS "${PROTOBUF_LIB_DIR}/libprotobufd.a")
    set(PROTOBUF_LIB_PATH "${PROTOBUF_LIB_DIR}/libprotobufd.a")
endif()

add_library(Protobuf::libprotobuf STATIC IMPORTED GLOBAL)
set_target_properties(Protobuf::libprotobuf PROPERTIES
    IMPORTED_LOCATION "${PROTOBUF_LIB_PATH}"
    INTERFACE_INCLUDE_DIRECTORIES "${PROTOBUF_INCLUDE_DIR}"
)
target_link_libraries(Core PUBLIC Protobuf::libprotobuf)



if (UNIX AND NOT APPLE)
    find_package(Threads REQUIRED)
    target_link_libraries(Core PUBLIC Threads::Threads dl)
endif()


# header include path
target_include_directories(Core PUBLIC
"${CMAKE_CURRENT_SOURCE_DIR}/Include/Protobuf"
"${CMAKE_CURRENT_SOURCE_DIR}"
"${CMAKE_CURRENT_SOURCE_DIR}"
"${CMAKE_CURRENT_SOURCE_DIR}/Job"
"${CMAKE_CURRENT_SOURCE_DIR}/Lock"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory/Allocator"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory/Buffer"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory/SmartPointer"
"${CMAKE_CURRENT_SOURCE_DIR}/Network"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/Epoll"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/IOCP"
"${CMAKE_CURRENT_SOURCE_DIR}/Protocol"
"${CMAKE_CURRENT_SOURCE_DIR}/Protocol/Protobuf"
"${CMAKE_CURRENT_SOURCE_DIR}/Thread"

)

target_precompile_headers(Core PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:pch.h>"
)

set_target_properties(Core PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../../Lib/Core/${CMAKE_BUILD_TYPE}/${ARCH_DIR}"
)


set(CORE_HEADERS_DST_ROOT "${ROOT_DIR}/Include/Core")

set(CORE_HEADERS_TO_COPY
"${CMAKE_CURRENT_SOURCE_DIR}/Container.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Job/GlobalQueue.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Job/Job.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Job/JobQueue.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Job/JobTimer.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Lock/CustomLock.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Lock/CustomReadLockGuard.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Lock/CustomWriteLockGuard.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Lock/DeadLockProfiler.h"
"${CMAKE_CURRENT_SOURCE_DIR}/LockQueue.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Macros.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory/Allocator/PoolAllocator.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory/Allocator/STLAllocator.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory/Buffer/RecvBuffer.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory/Buffer/SendBuffer.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory/Buffer/SendBufferChunk.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory/Buffer/SendBufferManager.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory/Memory.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory/MemoryPool.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory/ObjectPool.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Memory/SmartPointer/CustomEnableSharedFromThis.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/Epoll/EpollCore.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/Epoll/EpollEvent.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/Epoll/EpollListener.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/Epoll/EpollObject.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/Epoll/EpollSession.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/IOCP/IOCPCore.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/IOCP/IOCPEvent.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/IOCP/IOCPListener.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/IOCP/IOCPObject.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/IOCP/IOCPSession.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/NetAddress.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/NetCore.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/NetObject.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/PacketSession.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/Service.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Network/SocketUtil.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Protocol/Protobuf/Enum.pb.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Protocol/Protobuf/Protocol.pb.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Protocol/Protobuf/Struct.pb.h"
"${CMAKE_CURRENT_SOURCE_DIR}/ProtocolPacketHandler.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Structs.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Thread/ThreadManager.h"
"${CMAKE_CURRENT_SOURCE_DIR}/Types.h"
"${CMAKE_CURRENT_SOURCE_DIR}Global.h"
"${CMAKE_CURRENT_SOURCE_DIR}Pch.h"
"${CMAKE_CURRENT_SOURCE_DIR}TLS.h"

)

add_custom_command(TARGET Core POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${CORE_HEADERS_DST_ROOT}"
    COMMAND "${CMAKE_COMMAND}" -E echo "Copy selected core headers -> ${CORE_HEADERS_DST_ROOT}"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Container.h"
                    "${CORE_HEADERS_DST_ROOT}/Container.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Job/GlobalQueue.h"
                    "${CORE_HEADERS_DST_ROOT}/GlobalQueue.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Job/Job.h"
                    "${CORE_HEADERS_DST_ROOT}/Job.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Job/JobQueue.h"
                    "${CORE_HEADERS_DST_ROOT}/JobQueue.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Job/JobTimer.h"
                    "${CORE_HEADERS_DST_ROOT}/JobTimer.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Lock/CustomLock.h"
                    "${CORE_HEADERS_DST_ROOT}/CustomLock.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Lock/CustomReadLockGuard.h"
                    "${CORE_HEADERS_DST_ROOT}/CustomReadLockGuard.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Lock/CustomWriteLockGuard.h"
                    "${CORE_HEADERS_DST_ROOT}/CustomWriteLockGuard.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Lock/DeadLockProfiler.h"
                    "${CORE_HEADERS_DST_ROOT}/DeadLockProfiler.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/LockQueue.h"
                    "${CORE_HEADERS_DST_ROOT}/LockQueue.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Macros.h"
                    "${CORE_HEADERS_DST_ROOT}/Macros.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Memory/Allocator/PoolAllocator.h"
                    "${CORE_HEADERS_DST_ROOT}/PoolAllocator.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Memory/Allocator/STLAllocator.h"
                    "${CORE_HEADERS_DST_ROOT}/STLAllocator.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Memory/Buffer/RecvBuffer.h"
                    "${CORE_HEADERS_DST_ROOT}/RecvBuffer.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Memory/Buffer/SendBuffer.h"
                    "${CORE_HEADERS_DST_ROOT}/SendBuffer.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Memory/Buffer/SendBufferChunk.h"
                    "${CORE_HEADERS_DST_ROOT}/SendBufferChunk.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Memory/Buffer/SendBufferManager.h"
                    "${CORE_HEADERS_DST_ROOT}/SendBufferManager.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Memory/Memory.h"
                    "${CORE_HEADERS_DST_ROOT}/Memory.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Memory/MemoryPool.h"
                    "${CORE_HEADERS_DST_ROOT}/MemoryPool.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Memory/ObjectPool.h"
                    "${CORE_HEADERS_DST_ROOT}/ObjectPool.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Memory/SmartPointer/CustomEnableSharedFromThis.h"
                    "${CORE_HEADERS_DST_ROOT}/CustomEnableSharedFromThis.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Network/Epoll/EpollCore.h"
                    "${CORE_HEADERS_DST_ROOT}/EpollCore.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Network/Epoll/EpollEvent.h"
                    "${CORE_HEADERS_DST_ROOT}/EpollEvent.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Network/Epoll/EpollListener.h"
                    "${CORE_HEADERS_DST_ROOT}/EpollListener.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Network/Epoll/EpollObject.h"
                    "${CORE_HEADERS_DST_ROOT}/EpollObject.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Network/Epoll/EpollSession.h"
                    "${CORE_HEADERS_DST_ROOT}/EpollSession.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Network/IOCP/IOCPCore.h"
                    "${CORE_HEADERS_DST_ROOT}/IOCPCore.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Network/IOCP/IOCPEvent.h"
                    "${CORE_HEADERS_DST_ROOT}/IOCPEvent.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Network/IOCP/IOCPListener.h"
                    "${CORE_HEADERS_DST_ROOT}/IOCPListener.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Network/IOCP/IOCPObject.h"
                    "${CORE_HEADERS_DST_ROOT}/IOCPObject.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Network/IOCP/IOCPSession.h"
                    "${CORE_HEADERS_DST_ROOT}/IOCPSession.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Network/NetAddress.h"
                    "${CORE_HEADERS_DST_ROOT}/NetAddress.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Network/NetCore.h"
                    "${CORE_HEADERS_DST_ROOT}/NetCore.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Network/NetObject.h"
                    "${CORE_HEADERS_DST_ROOT}/NetObject.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Network/PacketSession.h"
                    "${CORE_HEADERS_DST_ROOT}/PacketSession.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Network/Service.h"
                    "${CORE_HEADERS_DST_ROOT}/Service.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Network/SocketUtil.h"
                    "${CORE_HEADERS_DST_ROOT}/SocketUtil.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Protocol/Protobuf/Enum.pb.h"
                    "${CORE_HEADERS_DST_ROOT}/Enum.pb.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Protocol/Protobuf/Protocol.pb.h"
                    "${CORE_HEADERS_DST_ROOT}/Protocol.pb.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Protocol/Protobuf/Struct.pb.h"
                    "${CORE_HEADERS_DST_ROOT}/Struct.pb.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/ProtocolPacketHandler.h"
                    "${CORE_HEADERS_DST_ROOT}/CorePacketHandler.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Structs.h"
                    "${CORE_HEADERS_DST_ROOT}/Structs.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Thread/ThreadManager.h"
                    "${CORE_HEADERS_DST_ROOT}/ThreadManager.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/Types.h"
                    "${CORE_HEADERS_DST_ROOT}/Types.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}Global.h"
                    "${CORE_HEADERS_DST_ROOT}/CoreGlobal.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}Pch.h"
                    "${CORE_HEADERS_DST_ROOT}/CorePch.h"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}TLS.h"
                    "${CORE_HEADERS_DST_ROOT}/CoreTLS.h"
    
    COMMENT "Copy headers -> ${CORE_HEADERS_DST_ROOT}"
    VERBATIM
)
