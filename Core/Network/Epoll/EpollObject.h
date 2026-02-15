#pragma once
#ifndef EPOLLOBJECT_H
#define EPOLLOBJECT_H

#if defined(__unix__) || defined(__unix) || defined(__linux__)


class EpollObject : public EnableSharedFromThis<EpollObject>
{
public:
    virtual int GetHandle() = 0;
    virtual void Dispatch(uint32 epollEvent) = 0;
};

#endif

#endif