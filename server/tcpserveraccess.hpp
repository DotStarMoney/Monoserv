/*
 * tcpserveraccess.hpp
 *
 *  Created on: Jan 4, 2016
 *      Author: ubuntu
 */

#ifndef TCPSERVERACCESS_HPP_
#define TCPSERVERACCESS_HPP_

#include "iasyncutility.hpp"

namespace Monospace
{
	namespace Server
	{
		class TCPServerAccess : public IAsyncUtility
		{
		public:
			TCPServerAccess(IAsyncUtility* _tcpserv)
			{
				tcpserv = _tcpserv;
			}

			void async_addTask(boost::function<void()> _task)
			{
				tcpserv->async_addTask(_task);
			}
			void async_sendData(  )
			{
				tcpserv->async_sendData(  );
			}

		private:
			IAsyncUtility* tcpserv;

		};
	}
}

#endif /* TCPSERVERACCESS_HPP_ */
