#pragma once
#ifndef JOB_H
#define JOB_H

#include "Types.h"

#if CPP_VERSION >= 11

class Job
{
public:
	Job(VoidCallback && callback)
		: _callback(std::move(callback))
	{
	}
	template<typename T, typename Ret, typename... Args>
	Job(SharedPtr<T> owner, Ret(T::* memFunc)(Args...), Args&&... args)
	{
		// owner	- (스마트 포인터)
		// Ret		- 멤버 함수의 반환 타입
		//		Ret(T::* memFunc)(Args...)	- T 타입의 멤버 함수 포인터
		//		void MyClass::Do(int, float); -> Ret = void, T = MyClass, Args... = {int, float}\
		//									  -> using MemFunc = void(MyClass::*)(int, float);
		// Args...	- 멤버 함수 인자의 타입 목록
		_callback = [owner, memFunc, args...]() {
			(owner.get()->*memFunc)(args...);
			};
	}
public:
	void Execute()
	{
		_callback();
	}
private:
	VoidCallback _callback;
};
#else

#endif // CPP_VERSION >= 11


#endif // JOB_H