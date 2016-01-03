/*
 * threadpool.hpp
 *
 *  Created on: Dec 31, 2015
 *      Author: Zamaster
 */

#ifndef THREADPOOL_HPP_
#define THREADPOOL_HPP_

#include "debug.hpp"
#include <queue>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

class ThreadPool
{
public:
    static const std::size_t DEFAULT_INIT_THREADS;
    static const std::size_t DEFAULT_MIN_THREADS;
    static const std::size_t DEFAULT_MAX_THREADS;
    static const unsigned int MAINTINENCE_INTERVAL_MS;
    static const unsigned int SPINDOWN_MS;

    enum Mode_e
	{
    	DYNAMIC,
		FIXED
	};

    ThreadPool()
    {
        construct();
        setPoolSize(DEFAULT_INIT_THREADS);
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
        DEBUG_PRINT("Shutting down, letting new threads block...");
        boost::this_thread::sleep(boost::posix_time::milliseconds(SPINDOWN_MS));
        threadBlockCondition.notify_all();
        maintinenceBlockCondition.notify_all();
        try
        {
            threads.join_all();
            maintinenceThread->join();
        }
        catch(...)
        {
        	DEBUG_PRINT("Exception: %s\n", boost::current_exception_diagnostic_information().c_str());
        }
        delete(maintinenceThread);
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

    void setMode(Mode_e _mode)
    {
    	boost::unique_lock<boost::mutex> lock(maintinenceBlockMutex);
    	mode = _mode;
    	if(mode == DYNAMIC) maintinenceBlockCondition.notify_all();
    }

private:

    Mode_e mode;
    int intervalCounter;
    std::size_t maxOpenThreads;
    std::size_t busyThreads;
    std::size_t intervalFreeThreads;
    boost::thread* maintinenceThread;
    std::queue< boost::function<void()> > tasks;
    boost::thread_group threads;
    bool shouldRun;
    bool queuedTasks;
    std::size_t threadMaxUtilization;
    std::size_t queueMinSize;
    boost::mutex threadBlockMutex;
    boost::condition_variable threadBlockCondition;
    boost::mutex maintinenceBlockMutex;
    boost::condition_variable maintinenceBlockCondition;

    void construct()
    {
        shouldRun = true;
        maxOpenThreads = 0;
        busyThreads = 0;
        intervalFreeThreads = 0;
        mode = FIXED;
        intervalCounter = 0;
        threadMaxUtilization = 0;
        queueMinSize = SIZE_MAX;
        queuedTasks = false;
        maintinenceThread = new boost::thread
        (
        	&ThreadPool::maintain,
			this
  		);
    }

    void maintain()
    {
    	boost::unique_lock<boost::mutex> lock(maintinenceBlockMutex);
    	do
    	{
    		if(mode == FIXED) maintinenceBlockCondition.wait(lock);
    		if(!shouldRun) break;
    		lock.unlock();
    		boost::this_thread::sleep(boost::posix_time::milliseconds(MAINTINENCE_INTERVAL_MS));
    		lock.lock();
    		if(!shouldRun) break;
    		if((intervalCounter & 1) == 1)
    		{
    			if((maxOpenThreads - threadMaxUtilization) >= 2)
    			{
    				setPoolSize(std::max(maxOpenThreads - 1, DEFAULT_MIN_THREADS));
    			}
    		}
    		if((queueMinSize > 1) && queuedTasks)
    		{
    			setPoolSize(std::min(maxOpenThreads + 1, DEFAULT_MAX_THREADS));
    		}
    		intervalCounter++;
    		threadMaxUtilization = 0;
    		queueMinSize = SIZE_MAX;
    		queuedTasks = false;
    	} while(shouldRun);
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

                maintinenceBlockMutex.lock();
                queuedTasks = true;
                if(tasks.size() < queueMinSize) queueMinSize = tasks.size();
                if(busyThreads > threadMaxUtilization) threadMaxUtilization = busyThreads;
                maintinenceBlockMutex.unlock();

                lock.unlock();
                try
                {
                    task();
                }
                catch(...)
                {
                	DEBUG_PRINT("Exception: %s\n", boost::current_exception_diagnostic_information().c_str());
                }
            }
            lock.lock();
            busyThreads--;
        } while(shouldRun && (threads.size() == maxOpenThreads));
        if(shouldRun) _tgroup.remove_thread(_thisThread);
    }
};




#endif /* THREADPOOL_HPP_ */
