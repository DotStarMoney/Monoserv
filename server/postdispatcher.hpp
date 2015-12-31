/*
 * postdispatcher.hpp
 *
 *  Created on: Dec 31, 2015
 *      Author: Zamaster
 */

#ifndef POSTDISPATCHER_HPP_
#define POSTDISPATCHER_HPP_

#include <stdlib.h>
#include "../threadpool.hpp"

namespace Server
{
    class POSTDispatcher
    {
    public:

    private:
        ThreadPool taskPool;
    };

}


#endif /* POSTDISPATCHER_HPP_ */
