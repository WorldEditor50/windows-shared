#ifndef CRITICALSECTION_HPP
#define CRITICALSECTION_HPP
#include <Windows.h>

class CriticalSection
{
public:
    class Lock
    {
    protected:
        CriticalSection &section;
    public:
        Lock(CriticalSection &section_):section(section_)
        {
            section.lock();
        }

        ~Lock()
        {
            section.unlock();
        }

    };
public:
    CRITICAL_SECTION section;
public:
    CriticalSection()
    {
        InitializeCriticalSection(&section);
    }
    ~CriticalSection()
    {
        DeleteCriticalSection(&section);
    }

    void lock()
    {
        EnterCriticalSection(&section);
        return;
    }

    void unlock()
    {
        LeaveCriticalSection(&section);
        return;
    }

};
#endif // CRITICALSECTION_HPP
