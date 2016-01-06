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
#include "socketaccess.hpp"

using boost::asio::ip::tcp;
namespace Monospace
{
	namespace Server
	{

		class TCPServer: private boost::noncopyable
		{
			typedef unsigned short Port;
			typedef void (*ServerTask)(SocketAccess&);
		public:
			TCPServer();
			TCPServer(Port _port, ServerTask _stask);
			void setTask(ServerTask _stask);
			void setPort(Port _port);
			void run();
		private:
			void construct();
			static void socketDispatch(ServerTask _stask, SocketAccess _access);

			ThreadPool taskPool;
			ServerTask stask;
			Port port;
		};

	}
}

#endif /* TCPSERVER_HPP_ */
