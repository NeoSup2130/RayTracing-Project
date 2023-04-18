#ifndef _THREADSAFEQUEUE_H_
#define _THREADSAFEQUEUE_H_

#include <queue>
#include <SDL.h>

template <class T>
class ThreadSafeQueue
{
    public:
        ThreadSafeQueue();
        ~ThreadSafeQueue();

        ThreadSafeQueue(ThreadSafeQueue* q) = delete;
        ThreadSafeQueue& operator=(ThreadSafeQueue& q) = delete;

        //Not thread safe getter!
        std::queue<T>& GetQueue() { return workQueue; };

        void push(T job);
        T pop(void);

        bool Empty() const;
private:
    std::queue<T> workQueue;
    SDL_mutex* lock;
    SDL_cond* available;

};

template <class T>
ThreadSafeQueue<T>::ThreadSafeQueue()
{
    lock = SDL_CreateMutex();
    available = SDL_CreateCond();
}

template <class T>
ThreadSafeQueue<T>::~ThreadSafeQueue()
{
    SDL_LockMutex(lock);
    while(!workQueue.empty())
        workQueue.pop();
    SDL_UnlockMutex(lock);

    SDL_DestroyMutex(lock);
    SDL_DestroyCond(available);
}

template<typename T>
bool ThreadSafeQueue<T>::Empty() const
{
    SDL_LockMutex(lock);
    bool result = workQueue.empty();
    SDL_UnlockMutex(lock);
    return result;
}

template <class T>
void ThreadSafeQueue<T>::push(T job)
{
    SDL_LockMutex(lock);
    workQueue.push(job);
    SDL_CondBroadcast(available);
    SDL_UnlockMutex(lock);
}

template <class T>
T ThreadSafeQueue<T>::pop(void)
{
    SDL_LockMutex(lock);
    while(workQueue.empty())
        SDL_CondWait(available,lock);
    T top = workQueue.front();
    workQueue.pop();
    SDL_UnlockMutex(lock);

    return top;
}

#endif
