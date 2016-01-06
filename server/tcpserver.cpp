/*
 * tcpserver.cpp
 *
 *  Created on: Jan 3, 2016
 *      Author: Zamaster
 */
#include "tcpserver.hpp"
#include <boost/bind.hpp>

#include "socketaccess.hpp"

using boost::asio::ip::tcp;

namespace Monospace
	{
	namespace Server
	{

		TCPServer::TCPServer()
		{
			construct();
		}

		TCPServer::TCPServer(Port _port, ServerTask _stask)
		{
			construct();
			setPort(_port);
			setTask(_stask);
		}

		void TCPServer::construct()
		{
			port = 0;
		}
		void TCPServer::setPort(Port _port)
		{
			port = _port;
		}
		void TCPServer::setTask(ServerTask _stask)
		{
			stask = _stask;
		}

		void TCPServer::run()
		{
			boost::asio::io_service IOProvider;
			tcp::acceptor connectIn(IOProvider, tcp::endpoint(tcp::v4(), port));
			tcp::socket* sockPtr;

			taskPool.setMode(ThreadPool::DYNAMIC);

			DEBUG_PRINT("Running TCP server...");

			for(;;)
			{
				sockPtr = new tcp::socket(IOProvider);
				connectIn.accept(*sockPtr);
				taskPool.addTask(boost::bind(&socketDispatch, stask, SocketAccess(sockPtr)));
			}

		}

		void TCPServer::socketDispatch(ServerTask _stask, SocketAccess _access)
		{
			DEBUG_PRINT("Serving incoming connection...");
			_stask(_access);
			_access.deleteSocket();
		}



	}
}
