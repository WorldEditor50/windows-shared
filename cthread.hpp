#ifndef CTHREAD_H
#define CTHREAD_H
#include <windows.h>
#include <process.h>

#pragma comment(lib, "User32.lib")

class CThread
{
public:
    enum State {
        STATE_NONE = 0,
        STATE_RUN,
        STATE_PAUSE,
        STATE_SUSPEND,
        STATE_TERMINATE
    };
    enum Property {
        PROPERTY_DEFAULT = 0,
        PROPERTY_SUSPEND = CREATE_SUSPENDED,
        PROPERTY_DETACHED = DETACHED_PROCESS,
        PROPERTY_NORMAL_PRIORITY = NORMAL_PRIORITY_CLASS,
        PROPERTY_IDLE_PRIORITY = IDLE_PRIORITY_CLASS,
        PROPERTY_HIGH_PRIORITY = NORMAL_PRIORITY_CLASS
    };
#if 0
    typedef unsigned __stdcall (*FnInvoke)(void * param);
#else
    typedef DWORD (WINAPI *FnInvoke)(LPVOID lpThreadParameter);
#endif
public:
    HANDLE handle;
    int state;
    DWORD address;
protected:
    static DWORD WINAPI routine(LPVOID params)
    {
        CThread *this_ = static_cast<CThread*>(params);
        return this_->run();
        return 0;
    }
    virtual int run(){return 0;}
public:
    CThread():handle(nullptr),state(STATE_NONE),address(0){}
    template<typename T>
    explicit CThread(FnInvoke func, T* obj, int property = PROPERTY_DEFAULT)
        :handle(nullptr),state(STATE_NONE),address(0)
    {
#if 0
        handle = (HANDLE)_beginthreadex(NULL,
                                        0,
                                        func,
                                        obj, property, &address);
#else
        handle = ::CreateThread(NULL,
                                0,
                                func,
                                obj, property, &address);
#endif
        if (handle == NULL) {
            return;
        }
        if (property == PROPERTY_SUSPEND) {
            state = STATE_SUSPEND;
        } else {
            state = STATE_RUN;
        }
    }

    virtual ~CThread()
    {
        if (handle != NULL) {
            CloseHandle(handle);
        }
    }

    unsigned int id() const { return address;}

    virtual int start(int property = PROPERTY_DEFAULT)
    {
        if (handle != NULL) {
            return -1;
        }
#if 0
        handle = (HANDLE)_beginthreadex(NULL,
                                        0,
                                        &routine,
                                        this, property, &address);
#else
        handle = ::CreateThread(NULL,
                                0,
                                &CThread::routine,
                                this, property, &address);
#endif
        if (handle == NULL) {
            return -2;
        }
        if (property == PROPERTY_SUSPEND) {
            state = STATE_SUSPEND;
        } else {
            state = STATE_RUN;
        }
        return 0;
    }

    virtual int suspend()
    {
        if (handle == nullptr || state == STATE_NONE) {
            return -1;
        }
        state = STATE_SUSPEND;
        DWORD ret = ::SuspendThread(handle);
        if (ret != 0) {
            return -2;
        }
        return 0;
    }

    virtual int resume()
    {
        if (handle == nullptr || state == STATE_NONE) {
            return -1;
        }
        state = STATE_RUN;
        DWORD ret = ::ResumeThread(handle);
        if (ret != 0) {
            return -2;
        }
        return 0;
    }

    virtual void join(int msec=-1)
    {
        if (handle == nullptr || state == STATE_NONE) {
            return;
        }

        int timeout = msec < 0 ? INFINITE : msec;
        DWORD ret = ::WaitForSingleObject(handle, timeout);
        if (ret == WAIT_OBJECT_0) {
            state = STATE_NONE;
        }
        return;
    }
    virtual int terminate()
    {
        if (handle == nullptr || state == STATE_NONE) {
            return -1;
        }
        DWORD exitCode =  0;
        BOOL ret = ::TerminateThread(handle, exitCode);
        if (ret == TRUE) {
            ::CloseHandle(handle);
            handle = nullptr;
            state = STATE_NONE;
            return 0;
        }
        return -2;
    }

};

#endif // CTHREAD_H
