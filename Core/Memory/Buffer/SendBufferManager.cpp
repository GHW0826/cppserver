#include "pch.h"
#include "SendBufferManager.h"
#include "SendBufferChunk.h"
#include "Memory.h"

SendBufferRef SendBufferManager::Open(uint32 size)
{
	if (LSendBufferChunk == nullptr) {
		LSendBufferChunk = Pop(); // WRITE_LOCK
		LSendBufferChunk->Reset();
	}

	ASSERT_CRASH(LSendBufferChunk->IsOpen() == false);

	// 다 썼으면 버리고 새거로 교체
	if (LSendBufferChunk->FreeSize() < size) {
		LSendBufferChunk = Pop(); // WRITE_LOCK
		LSendBufferChunk->Reset();
	}

	return LSendBufferChunk->Open(size);
}

SendBufferChunkRef SendBufferManager::Pop()
{
	{
		WRITE_LOCK;
		if (_sendBufferChunks.empty() == false) {
			SendBufferChunkRef sendBufferChunk = _sendBufferChunks.back();
			_sendBufferChunks.pop_back();
			return sendBufferChunk;
		}
	}

	return SendBufferChunkRef(snew<SendBufferChunk>(), PushGlobal);
}

void SendBufferManager::Push(SendBufferChunkRef buffer)
{
	WRITE_LOCK;
	_sendBufferChunks.push_back(buffer);
}

void SendBufferManager::PushGlobal(SendBufferChunk* buffer)
{
	std::cout << "PushGlobal SENDBUFFERCHUNK" << std::endl;

	GSendBufferManager->Push(SendBufferChunkRef(buffer, PushGlobal));
}