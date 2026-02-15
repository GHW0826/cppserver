#pragma once
#ifndef GLOBALQUEUE_H
#define GLOBALQUEUE_H

#include "LockQueue.h"

class GlobalQueue
{
public:
	GlobalQueue();
	~GlobalQueue();

	void					Push(JobQueueRef jobQueue);
	JobQueueRef				Pop();
private:
	LockQueue<JobQueueRef> _jobQueues;
};


#endif // GLOBALQUEUE_H