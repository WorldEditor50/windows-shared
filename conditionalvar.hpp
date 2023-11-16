#ifndef CONDITIONALVAR_HPP
#define CONDITIONALVAR_HPP
#include <Windows.h>
#include "srwlock.hpp"
#include "criticalsection.hpp"

class ConditionalVar
{
public:
    typedef bool (*FnConditon)();
protected:
    CONDITION_VARIABLE  var;
public:

    ConditionalVar()
    {
        InitializeConditionVariable(&var);
    }

    ~ConditionalVar()
    {

    }

    void wait(SRWLock &srwLock, const FnConditon condition, int msec=-1)
    {
        int timeout = msec < 0 ? INFINITE : msec;
        while (!condition()) {
            SleepConditionVariableSRW(&var,
                                      &srwLock.lock,
                                      timeout,
                                      CONDITION_VARIABLE_LOCKMODE_SHARED);
        }
        return;
    }

    void wait(CriticalSection &cs, const FnConditon condition, int msec=-1)
    {
        int timeout = msec < 0 ? INFINITE : msec;
        while (!condition()) {
            SleepConditionVariableCS(&var,
                                     &cs.section,
                                     timeout);
        }
        return;
    }

    template<class T>
    void wait(SRWLock &srwLock, const T *obj, int msec=-1)
    {
        int timeout = msec < 0 ? INFINITE : msec;
        while (!obj->isReady()) {
            SleepConditionVariableSRW(&var,
                                      &srwLock.lock,
                                      timeout,
                                      CONDITION_VARIABLE_LOCKMODE_SHARED);
        }
        return;
    }

    template<class T>
    void wait(CriticalSection &cs, const T *obj, int msec=-1)
    {
        int timeout = msec < 0 ? INFINITE : msec;
        while (!obj->isReady()) {
            SleepConditionVariableCS(&var,
                                     &cs.section,
                                     timeout);
        }
        return;
    }

    void notify()
    {
        WakeConditionVariable(&var);
        return;
    }

    void notifyAll()
    {
        WakeAllConditionVariable(&var);
        return;
    }
};

#endif // CONDITIONALVAR_HPP
