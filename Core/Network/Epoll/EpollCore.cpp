#include "pch.h"

#if defined(__unix__) || defined(__unix) || defined(__linux__)

#include "EpollCore.h"
#include "EpollObject.h"


EpollCore::EpollCore()
{
	_epollHandle = epoll_create1(0);
	ASSERT_CRASH(_epollHandle != -1);

	_events.resize(_maxEvents);
}

EpollCore::~EpollCore()
{
    if (_epollHandle != -1) {
        close(_epollHandle);
    }
}

bool EpollCore::Register(EpollObjectRef obj)
{
    // 커널은 *ev 내용을 복사해서 내부에 저장
    // ev가 로컬이어도 전혀 문제 없음
	epoll_event ev{};
	ev.events = EPOLLIN;
    // Listener는 Service에 참조되어있음
    // 다른 EpollEvent들은 Session에 참조 되어 있음
	ev.data.ptr = obj.get();
	return ::epoll_ctl(_epollHandle, EPOLL_CTL_ADD, obj->GetHandle(), &ev) == 0;
}

bool EpollCore::Dispatch(uint32 timeoutMs)
{
    int eventCnt = ::epoll_wait(_epollHandle, _events.data(), _maxEvents, (int)timeoutMs);
    // timeout
    if (eventCnt == 0) {
        return false;
    } else if (eventCnt < 0) {
        // signal interrupt면 그냥 루프 지속
        if (errno == EINTR) {
            return false;
        }
        return false;
    }

    for (int i = 0; i < eventCnt; ++i) {
        // ptr은 event class
        // EpollObjectRef epollObject = _events[i].data.ptr;
        EpollObject* epollObject = (EpollObject*)_events[i].data.ptr;
        epollObject->Dispatch(_events[i].events);
    }
    return true;
}

#endif