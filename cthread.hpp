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

    enum JobState {
        JOB_PREPEND = 0,
        JOB_FINISHED
    };

public:
    HANDLE handle;
    int state;
    int jobState;
    DWORD address;
protected:
    static unsigned __stdcall startAddress(void * obj)
    {
        CThread* this_ = static_cast<CThread*>(obj);
        if (this_ != nullptr) {
            this_->run();
            this_->setJob(JOB_FINISHED);
        }
        return 0;
    }

    virtual void run(){}
public:
    CThread():handle(nullptr),state(STATE_NONE),jobState(JOB_PREPEND),address(0){}

    virtual ~CThread()
    {
        if (jobState != JOB_PREPEND) {
            join(-1);
        }
        if (handle != NULL) {
            CloseHandle(handle);
        }
    }

    unsigned int id() const { return address;}

    virtual int start(int property = PROPERTY_DEFAULT)
    {
        if (state != STATE_NONE) {
            return 0;
        }
#if 0
        handle = (HANDLE)_beginthreadex(NULL, 0, CThread::startAddress, this, property, &address);
#else
        handle = ::CreateThread(NULL,
                                0,
                                (LPTHREAD_START_ROUTINE)CThread::startAddress,
                                this,
                                property,
                                &address);
#endif
        if (handle == NULL) {
            return -1;
        }
        if (property == PROPERTY_SUSPEND) {
            state = STATE_SUSPEND;
        } else {
            state = STATE_RUN;
        }
        jobState = JOB_PREPEND;
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

    virtual void join(int timeout=-1)
    {
        if (handle == nullptr || state == STATE_NONE) {
            return;
        }
        if (timeout <= 0) {
            timeout = INFINITE;
        }
        DWORD ret = ::WaitForSingleObject(handle, timeout);
        if (ret == WAIT_OBJECT_0) {
            jobState = JOB_PREPEND;
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

    virtual void setJob(JobState jobState_)
    {
        jobState = jobState_;
        return;
    }

};

#endif // CTHREAD_H
