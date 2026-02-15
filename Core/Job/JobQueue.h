#pragma once
#ifndef JOBQUEUE_H
#define JOBQUEUE_H

#include "Job.h"
#include "Types.h"
#include "JobTimer.h"
#include "LockQueue.h"

class JobQueue : public EnableSharedFromThis<JobQueue>
{
public:
	void DoAsync(VoidCallback&& callback)
	{
		Push(ObjectPool<Job>::MakeShared(std::move(callback)));
	}

	template<typename T, typename Ret, typename... Args>
	void DoAsync(Ret(T::* memFunc)(Args...), Args... args)
	{
		SharedPtr<T> owner = static_pointer_cast<T>(shared_from_this());
		Push(ObjectPool<Job>::MakeShared(owner, memFunc, std::forward<Args>(args)...));
	}

	void DoTimer(uint64 tickAfter, VoidCallback&& callback)
	{
		JobRef job = ObjectPool<Job>::MakeShared(std::move(callback));
		GJobTimer->Reserve(tickAfter, shared_from_this(), job);
	}

	template<typename T, typename Ret, typename... Args>
	void DoTimer(uint64 tickAfter, Ret(T::* memFunc)(Args...), Args... args)
	{
		SharedPtr<T> owner = static_pointer_cast<T>(shared_from_this());
		JobRef job = ObjectPool<Job>::MakeShared(owner, memFunc, std::forward<Args>(args)...);
		GJobTimer->Reserve(tickAfter, shared_from_this(), job);
	}

	void					ClearJobs() { _jobs.Clear(); }
public:
	void					Push(JobRef job, bool pushOnly = false);
	void					Execute();
protected:
	LockQueue<JobRef>		_jobs;
	Atomic<int32>			_jobCount = 0;
};



#endif // JOBQUEUE_H