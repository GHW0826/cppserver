#pragma once
#ifndef IOCPOBJECT_H
#define IOCPOBJECT_H
#if defined(_WIN32) || defined(_WIN64)


class IOCPObejct : public std::enable_shared_from_this<IOCPObejct>
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IOCPEvent* iocpEvent, int32 numOfBytes = 0) abstract;
};

#endif // win
#endif // IOCPBOBJECT_H
