#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP
#include <Windows.h>
#include <cstdio>


class Semaphore
{
public:
    class Guard
    {
    protected:
        Semaphore &sem;

    public:
        Guard(Semaphore &sem_, int timeout=0):sem(sem_)
        {
            sem.wait(timeout);
        }
        ~Guard()
        {
            sem.post();
        }
    };

protected:
    HANDLE handle;
    DWORD result;
public:
    Semaphore(int initCount, int maxCount):handle(nullptr)
    {
        // Create a semaphore with initial and max counts of MAX_SEM_COUNT
         handle = CreateSemaphore(NULL,           // default security attributes
                                  initCount,      // initial count
                                  maxCount,       // maximum count
                                  NULL);          // unnamed semaphore

    }

    ~Semaphore()
    {
        if (handle != nullptr) {
            CloseHandle(handle);
            handle = nullptr;
        }
    }

    void wait(int timeout=0)
    {
        // Try to enter the semaphore gate.
        result = WaitForSingleObject(handle, timeout);
        switch (result) {
            // The semaphore object was signaled.
            case WAIT_OBJECT_0:
                break;

            // The semaphore was nonsignaled, so a time-out occurred.
            case WAIT_TIMEOUT:
                printf("Thread %d: wait timed out\n", GetCurrentThreadId());
                break;
        }

        return;
    }

    void post()
    {
        if (result == WAIT_OBJECT_0) {
            // Release the semaphore when task is finished
            if (!ReleaseSemaphore(handle,  // handle to semaphore
                                  1,            // increase count by one
                                  NULL)) {      // not interested in previous count

                printf("ReleaseSemaphore error: %d\n", GetLastError());
            }
        }
        return;
    }
};

#endif // SEMAPHORE_HPP
