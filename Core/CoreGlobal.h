#pragma once
#ifndef COREGLOBAL_H
#define COREGLOBAL_H

extern class ThreadManager* GThreadManager;
extern class Memory* GMemory;
extern class DeadLockProfiler* GDeadLockProfiler;
extern class JobTimer* GJobTimer;
extern class GlobalQueue* GGlobalQueue;
extern class SendBufferManager* GSendBufferManager;

#endif // COREGLOBAL_H
