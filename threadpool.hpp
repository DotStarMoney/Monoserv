/*
 * threadpool.hpp
 *
 *  Created on: Dec 31, 2015
 *      Author: Zamaster
 */

#ifndef THREADPOOL_HPP_
#define THREADPOOL_HPP_


#include <queue>
#include <vector>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

class ThreadPool
{
public:
    static const std::size_t DEFAULT_MAX_THREADS;

    ThreadPool()
    {
        construct();
        setPoolSize(DEFAULT_MAX_THREADS);
    }
    ThreadPool(std::size_t _threads_n)
    {
        construct();
        setPoolSize(_threads_n);
    }

    ~ThreadPool()
    {
        {
            boost::unique_lock<boost::mutex> lock(threadBlockMutex);
            shouldRun = false;
        }
        threadBlockCondition.notify_all();
        try
        {
            threads.join_all();
        }
        catch(...)
        {
            //
        }
    }

    void setPoolSize(std::size_t _threads_n)
    {
        unsigned int i;
        int endNThreads;
        boost::thread* newThread;
        if(_threads_n > maxOpenThreads)
        {
            threadBlockMutex.lock();
            for(i = maxOpenThreads; i < _threads_n; i++)
            {
                newThread = new boost::thread();
                *newThread = boost::thread(boost::bind
                (
                    &ThreadPool::threadLoop,
                    this,
                    boost::ref(threads),
                    newThread
                ));
                threads.add_thread(newThread);
            }
            maxOpenThreads = _threads_n;
            threadBlockMutex.unlock();
        }
        else if (_threads_n < maxOpenThreads)
        {
            threadBlockMutex.lock();
            endNThreads = (maxOpenThreads - _threads_n) - busyThreads;
            if(endNThreads < 0) endNThreads = 0;
            maxOpenThreads = _threads_n;
            threadBlockMutex.unlock();
            for(i = 0; i < (unsigned int) endNThreads; i++)
            {
                threadBlockCondition.notify_one();
            }
        }
    }
    std::size_t getPoolSize()
    {
        return maxOpenThreads;
    }

    template <typename TaskType>
    void addTask(TaskType _task)
    {
        boost::unique_lock<boost::mutex> lock(threadBlockMutex);

        tasks.push(boost::function<void()> (_task));
        if(busyThreads < threads.size()) threadBlockCondition.notify_one();
    }

private:

    std::size_t maxOpenThreads;
    std::size_t busyThreads;
    std::queue< boost::function<void()> > tasks;
    boost::thread_group threads;
    bool shouldRun;
    boost::mutex threadBlockMutex;
    boost::condition_variable threadBlockCondition;

    void construct()
    {
        shouldRun = true;
        maxOpenThreads = 0;
        busyThreads = 0;
    }

    void threadLoop(boost::thread_group& _tgroup, boost::thread* _thisThread)
    {
        boost::unique_lock<boost::mutex> lock(threadBlockMutex);
        do
        {
            if(tasks.empty()) threadBlockCondition.wait(lock);
            if((threads.size() > maxOpenThreads) || !shouldRun) break;
            {
                boost::function<void()> task = tasks.front();
                tasks.pop();
                busyThreads++;
                lock.unlock();
                try
                {
                    task();
                }
                catch(...)
                {
                    //
                }
            }
            lock.lock();
            busyThreads--;
        } while(shouldRun && (threads.size() == maxOpenThreads));
        if(shouldRun) _tgroup.remove_thread(_thisThread);
    }
};




#endif /* THREADPOOL_HPP_ */
