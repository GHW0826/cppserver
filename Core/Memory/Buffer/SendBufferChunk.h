#pragma once
#ifndef SENDBUFFERCHUNK_H
#define SENDBUFFERCHUNK_H

#include "Types.h"
#include "Container.h"

class SendBufferChunk : public EnableSharedFromThis<SendBufferChunk>
{
	enum {
		SEND_BUFFER_CHUNK_SIZE = 6000
	};
public:
	SendBufferChunk();
	~SendBufferChunk();
public:
	void				Reset();
	SendBufferRef		Open(uint32 allocSize);
	void				Close(uint32 writeSize);
public:
	bool				IsOpen() { return _open; }
	BYTE* Buffer() { return &_buffer[_usedSize]; }
	uint32				FreeSize() { return static_cast<uint32>(_buffer.size()) - _usedSize; }
private:
	Array<BYTE, SEND_BUFFER_CHUNK_SIZE>		_buffer = {};
	bool									_open = false;
	uint32									_usedSize = 0;
};

#endif // SENDBUFFERCHUNK_H