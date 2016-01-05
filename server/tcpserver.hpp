/*
 * tcpserver.hpp
 *
 *  Created on: Dec 31, 2015
 *      Author: Zamaster
 */

#ifndef TCPSERVER_HPP_
#define TCPSERVER_HPP_
#include <boost/asio.hpp>
#include <boost/core/noncopyable.hpp>
#include "../threadpool.hpp"
#include "tcpserveraccess.hpp"

using boost::asio::ip::tcp;
namespace Monospace
{
	namespace Server
	{

		class TCPServer: private boost::noncopyable, private IAsyncUtility
		{
			friend TCPServerAccess;
			typedef unsigned short Port;
			typedef void (*ServerTask)(tcp::socket&, TCPServerAccess&);
		public:
			TCPServer();
			TCPServer(Port _port, ServerTask _stask);
			void setTask(ServerTask _stask);
			void setPort(Port _port);
			void run();
		private:
			void construct();
			TCPServerAccess getAccess();
			static void socketDispatch(ServerTask _stask, tcp::socket* _socketPtr, TCPServerAccess _access);

			void async_addTask(boost::function<void()> _task)
			{
				taskPool.addTask(_task);
			}
			void async_sendData( /* parameter */ )
			{
				//
			}

			ThreadPool taskPool;
			ServerTask stask;
			Port port;
		};

	}
}

#endif /* TCPSERVER_HPP_ */
