/*
 * threadpool.cpp
 *
 *  Created on: Dec 31, 2015
 *      Author: Zamaster
 */


#include "threadpool.hpp"

const std::size_t ThreadPool::DEFAULT_INIT_THREADS = 4;
const std::size_t ThreadPool::DEFAULT_MIN_THREADS = 2;
const std::size_t ThreadPool::DEFAULT_MAX_THREADS = 8;
const unsigned int ThreadPool::MAINTINENCE_INTERVAL_MS = 3000;
const unsigned int ThreadPool::SPINDOWN_MS = 1000;
