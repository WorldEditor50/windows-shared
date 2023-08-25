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

    void wait(SRWLock &srwLock, const FnConditon condition, int timeout=-1)
    {
        int timeout_ = timeout;
        if (timeout < 0) {
            timeout_ = INFINITE;
        }

        while (condition() == false) {
            SleepConditionVariableSRW(&var,
                                      &srwLock.lock,
                                      timeout_,
                                      CONDITION_VARIABLE_LOCKMODE_SHARED);
        }
        return;
    }

    void wait(CriticalSection &cs, const FnConditon condition, int timeout=-1)
    {
        int timeout_ = timeout;
        if (timeout < 0) {
            timeout_ = INFINITE;
        }
        while (condition() == false) {
            SleepConditionVariableCS(&var,
                                     &cs.section,
                                     timeout_);
        }
        return;
    }

    template<class T>
    void wait(SRWLock &srwLock, const T *obj, int timeout=-1)
    {
        int timeout_ = timeout;
        if (timeout < 0) {
            timeout_ = INFINITE;
        }

        while (!obj->isProceed()) {
            SleepConditionVariableSRW(&var,
                                      &srwLock.lock,
                                      timeout_,
                                      CONDITION_VARIABLE_LOCKMODE_SHARED);
        }
        return;
    }

    template<class T>
    void wait(CriticalSection &cs, const T *obj, int timeout=-1)
    {
        int timeout_ = timeout;
        if (timeout < 0) {
            timeout_ = INFINITE;
        }
        while (!obj->isProceed()) {
            SleepConditionVariableCS(&var,
                                     &cs.section,
                                     timeout_);
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
