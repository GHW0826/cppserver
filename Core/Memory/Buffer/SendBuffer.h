#pragma once
#ifndef SENDBUFFER_H
#define SENDBUFFER_H

#include "Types.h"

class SendBufferChunk;

class SendBuffer
{
public:
	SendBuffer(SendBufferChunkRef owner, BYTE* buffer, uint32 allocSize);
	~SendBuffer();

	BYTE* Buffer() { return _buffer; }
	uint32		AllocSize() { return _allocSize; }
	uint32		WriteSize() { return _writeSize; }
	void		Close(uint32 writeSize);

private:
	BYTE* _buffer;
	uint32				_allocSize = 0;
	uint32				_writeSize = 0;
	SendBufferChunkRef	_owner;
};

#endif // SENDBUFFER_H