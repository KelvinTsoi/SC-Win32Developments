#ifndef __LOCK_H__
#define __LOCK_H__

#ifdef  WIN32
#include <windows.h>
#include <assert.h>
#else
#include <pthread.h>
#include <unistd.h> 
#include <errno.h> 
#include <sys/ipc.h> 
#include <semaphore.h> 
#endif


class OSLock
{
private:
#ifdef WIN32
    CRITICAL_SECTION lock_;
#else
    pthread_mutex_t lock_;
#endif //WIN32

public:

    OSLock()
    {
#ifdef WIN32
        InitializeCriticalSection(&lock_);
#else
        pthread_mutex_init(&lock_, 0);
#endif //WIN32
    }

    ~OSLock()
    {
#ifdef WIN32
        DeleteCriticalSection(&lock_);
#else
        pthread_mutex_destroy(&lock_);
#endif //WIN32
    }

    void Lock()
    {
#ifdef WIN32
        EnterCriticalSection(&lock_);
#else
        pthread_mutex_lock(&lock_);
#endif //WIN32
    }

    void Unlock()
    {
#ifdef WIN32
        LeaveCriticalSection(&lock_);
#else
        pthread_mutex_unlock(&lock_);
#endif //WIN32
    }

    class LockOwner
    {
    protected:
        OSLock& lock_;

    public:

        LockOwner(OSLock& Lock) : lock_(Lock)
        {
            lock_.Lock();
        }

        ~LockOwner()
        {
            lock_.Unlock();
        }
    };
};
#endif //__LOCK_H__
