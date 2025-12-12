#pragma once
#ifndef DEADLOCKPROFILER_H
#define DEADLOCKPROFILER_H

class DeadLockProfiler
{
public:
	void PushLock(const char* name);
	void PopLock(const char* name);
	void CheckCycle();
private:
	void Dfs(int32 index);
private:
	HashMap<const char*, int32> _nameToId;
	HashMap<int32, const char*> _idToName;
	Map<int32, Set<int32>> _lockHistory;

	Mutex _lock;
private:
	// 노드가 발견된 순서를 기록하는 배열
	Vector<int32> _discoveredOrder;
	// 노드가 발견된 순서
	int32 _discoveredCount = 0;
	// Dfs(i)가 종료 되었는지 여부
	Vector<bool> _finished;
	Vector<int32> _parent;
};


#endif // DEADLOCKPROFILER_H
