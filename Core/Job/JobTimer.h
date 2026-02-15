#pragma once
#ifndef JOBTIMER_H
#define JOBTIMER_H


#include "Types.h"

class JobTimer
{
public:
	void Reserve(uint64 tickAfter, WeakedPtr<JobQueue> owner, JobRef job);
	void Distribute(uint64 now);
	void Clear();

private:
	USE_LOCK;
	PriorityQueue<TimerItem> _items;
	Atomic<bool> _distributing = false;
};



#endif JOBTIMER_H
