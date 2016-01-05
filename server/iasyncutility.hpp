/*
 * iasyncutility.hpp
 *
 *  Created on: Jan 5, 2016
 *      Author: ubuntu
 */

#ifndef IASYNCUTILITY_HPP_
#define IASYNCUTILITY_HPP_

#include <boost/function.hpp>

namespace Monospace
{
	namespace Server
	{
		class IAsyncUtility
		{
		public:
			virtual ~IAsyncUtility() {};

			virtual void async_addTask(boost::function<void()> _task) = 0;
			virtual void async_sendData( /* parameter */ ) = 0;

		};
	}
}

#endif /* IASYNCUTILITY_HPP_ */
