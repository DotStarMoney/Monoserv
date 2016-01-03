/*
 * tcpserver.cpp
 *
 *  Created on: Jan 3, 2016
 *      Author: Zamaster
 */
#include "tcpserver.hpp"
#include "../threadpool.hpp"
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;

namespace Server
{
	TCPServer::TCPServer()
	{
		construct();
	}

	TCPServer::TCPServer(Port _port)
	{
		construct();
		setPort(_port);
	}

	void TCPServer::construct()
	{
		port = 0;
	}

	void TCPServer::setPort(Port _port)
	{
		port = _port;
	}

	void TCPServer::run()
	{
		ThreadPool taskPool;
		boost::asio::io_service IOProvider;
		tcp::acceptor connectIn(IOProvider, tcp::endpoint(tcp::v4(), port));
		tcp::socket* sockPtr;

		taskPool.setMode(ThreadPool::DYNAMIC);

		DEBUG_PRINT("Running TCP server...");

		for(;;)
		{
			sockPtr = new tcp::socket(IOProvider);
			connectIn.accept(*sockPtr);
			taskPool.addTask(boost::bind(socketDispatcher, sockPtr));
		}

	}
    void TCPServer::socketDispatcher(tcp::socket* _socketPtr)
    {
    	taskMain(*_socketPtr);
    	delete(_socketPtr);
    }
    void TCPServer::taskMain(tcp::socket& _socket)
    {
    	// need to get task from somewhere
    }


}
