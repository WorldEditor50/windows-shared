#ifndef CTIMER_HPP
#define CTIMER_HPP
#include <Windows.h>
#include <ctime>
#include "criticalsection.hpp"
#include "conditionalvar.hpp"
#include "cthread.hpp"

class CTimer : public CThread
{
public:
    typedef int (*FnOnTimeout)();
protected:
    unsigned long long startTime;
    int timeout;
    FnOnTimeout onTimeout;
    CriticalSection cs;
    ConditionalVar condit;
protected:

    static unsigned long long getSecondTimestamp()
    {
        unsigned long long timestamp;
        SYSTEMTIME currentTime;
        GetLocalTime(&currentTime);
        struct tm t = {currentTime.wSecond,
                       currentTime.wMinute,
                       currentTime.wHour,
                       currentTime.wDay,
                       currentTime.wMonth - 1,
                       currentTime.wYear - 1900};
        timestamp = mktime(&t);
        return timestamp;
    }


    virtual void run() override
    {
        while (1) {
            CriticalSection::Lock locker(cs);
            condit.wait(cs, this, 1000);
            if (state == STATE_TERMINATE) {
                state = STATE_NONE;
                break;
            }

            unsigned long long endTime = getSecondTimestamp();
            int duration = endTime - startTime;
            if (duration >= timeout) {
                if (onTimeout != nullptr) {
                    onTimeout();
                }
                break;
            }
            //Sleep(3000);
        }
        return;
    }

public:
    CTimer():timeout(10),onTimeout(nullptr){}
    ~CTimer()
    {
        stop();
    }
    bool isProceed() const
    {
        return state == STATE_RUN || state == STATE_TERMINATE;
    }
    void startAfter(int sec, const FnOnTimeout onTimeout_)
    {
        timeout = sec;
        onTimeout = onTimeout_;
        startTime = getSecondTimestamp();
        CThread::start(PROPERTY_DEFAULT);
        return;
    }

    void stop()
    {
        if (state == STATE_NONE) {
            return;
        }
        {
            CriticalSection::Lock locker(cs);
            state = STATE_TERMINATE;
            condit.notify();
        }
        CThread::join(3000);
        return;
    }


};

#endif // CTIMER_HPP
