#pragma once
#ifndef STRUCTS_H
#define STRUCTS_H

struct PacketHeader
{
	uint16 size;
	uint16 id; // 프로토콜ID (ex. 1=로그인, 2=이동요청)
};

struct JobData 
{
	JobData(WeakedPtr<JobQueue> owner, JobRef job) : owner(owner), job(job)
	{}

	WeakedPtr<JobQueue>	owner;
	JobRef				job;
};

struct TimerItem
{
	bool operator<(const TimerItem& other) const
	{
		return executeTick > other.executeTick;
	}

	uint64 executeTick = 0;
	JobData* jobData = nullptr;
};

#endif // STRUCTS_H