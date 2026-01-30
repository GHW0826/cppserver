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


struct Tick_Windows
{
    static inline uint64 NowMs()
    {
#if defined(_WIN32) || defined(_WIN64)
        return static_cast<uint64>(::GetTickCount64());
#else
        return 0;
#endif
    }
};

struct Tick_Linux
{
    static inline uint64 NowMs()
    {
#if defined(__unix__) || defined(__unix) || defined(__linux__)
        timespec ts{};
        ::clock_gettime(CLOCK_MONOTONIC, &ts); // 단조 증가
        return static_cast<uint64>(ts.tv_sec) * 1000ULL
            + static_cast<uint64>(ts.tv_nsec) / 1000000ULL;
#else
        return 0;
#endif
    }
};

#endif // STRUCTS_H