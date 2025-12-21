#pragma once
#ifndef SENDBUFFERMANAGER_H
#define SENDBUFFERMANAGER_H

class SendBufferManager
{
public:
	SendBufferRef		Open(uint32 size);

private:
	SendBufferChunkRef	Pop();
	void				Push(SendBufferChunkRef buffer);

	static void			PushGlobal(SendBufferChunk* buffer);
private:
	USE_LOCK
	Vector<SendBufferChunkRef> _sendBufferChunks;
};


#endif // SENDBUFFERMANAGER_H