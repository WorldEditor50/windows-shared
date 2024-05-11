#ifndef CMUTEX_HPP
#define CMUTEX_HPP
#include <Windows.h>
#include <string>

class CMutex
{
public:
    class Lock
    {
    protected:
        CMutex &mutex;
    public:
        Lock(CMutex &mutex_):mutex(mutex_)
        {
            mutex.lock();
        }
        ~Lock()
        {
            mutex.unlock();
        }
    };
protected:
    HANDLE handle;
    DWORD result;
private:
    CMutex(const CMutex &);
    CMutex& operator=(const CMutex &);
public:
    CMutex()
    {
        handle = CreateMutex(NULL, FALSE, NULL);
    }
    ~CMutex()
    {
        if (handle != nullptr) {
            CloseHandle(handle);
        }
    }
    void lock(int timeout=-1)
    {
        if (timeout < 0) {
            timeout = INFINITE;
        }
        result = ::WaitForSingleObject(handle, timeout);
        switch (result) {
        case WAIT_OBJECT_0:
            break;
        case WAIT_ABANDONED:
            break;
        }
        return;
    }

    void unlock()
    {
        if (result == WAIT_OBJECT_0) {
            ReleaseMutex(handle);
        }
        return;
    }
};

#endif // CMUTEX_HPP
