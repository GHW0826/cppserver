#pragma once
#ifndef CORETLS_H
#define CORETLS_H

#include <stack>

extern TLS uint32 LThreadId;
extern TLS uint64 LEndTickCount;

extern thread_local std::stack<int32> LLockStack;
// extern thread_local SendBufferChunkRef LSendBufferChunk;
extern thread_local class JobQueue* LCurrentJobQueue;

#endif // !CORETLS_H
