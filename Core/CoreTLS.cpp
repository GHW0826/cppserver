#include "pch.h"
#include "CoreTLS.h"

TLS uint32 LThreadId = 0;
TLS uint64 LEndTickCount = 0;
TLS Stack<int32>	LLockStack;
TLS SendBufferChunkRef	LSendBufferChunk;
TLS JobQueue* LCurrentJobQueue = nullptr;