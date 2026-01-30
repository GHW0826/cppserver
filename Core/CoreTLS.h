#pragma once
#ifndef CORETLS_H
#define CORETLS_H

#include "Types.h"
#include "Container.h"

extern TLS uint32 LThreadId;
extern TLS uint64 LEndTickCount;
extern TLS Stack<int32> LLockStack;
extern TLS SendBufferChunkRef LSendBufferChunk;
extern TLS class JobQueue* LCurrentJobQueue;

#endif // !CORETLS_H
