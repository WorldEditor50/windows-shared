#ifndef SRWLOCK_HPP
#define SRWLOCK_HPP
#include <Windows.h>

class SRWLock
{
public:
    /* 写数据时上锁，其他线程无法读写 */
    class Exclusive
    {
    public:
        SRWLock &rwLock;
    public:
        Exclusive(SRWLock &rwLock_):rwLock(rwLock_)
        {
            rwLock.exclusiveLock();
        }
        ~Exclusive()
        {
            rwLock.exclusiveUnlock();
        }
    };
    /* 读数据时上锁，其他线程无法写，但仍可读 */
    class Shared
    {
    public:
        SRWLock &rwLock;
    public:
        Shared(SRWLock &rwLock_):rwLock(rwLock_)
        {
            rwLock.sharedLock();
        }
        ~Shared()
        {
            rwLock.sharedUnlock();
        }
    };
public:
    SRWLOCK  lock;
public:
    SRWLock()
    {
        InitializeSRWLock(&lock);
    }

    ~SRWLock()
    {

    }
    void exclusiveLock()
    {
        AcquireSRWLockExclusive(&lock);
    }

    void exclusiveUnlock()
    {
        ReleaseSRWLockExclusive(&lock);
    }

    void sharedLock()
    {
        AcquireSRWLockShared(&lock);
    }

    void sharedUnlock()
    {
        ReleaseSRWLockShared(&lock);
    }
};
#endif // SRWLOCK_HPP
