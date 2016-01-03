/*
 * main.cpp
 *
 *  Created on: Dec 31, 2015
 *      Author: ubuntu
 */
#include "debug.hpp"
#include "server/tcpserver.hpp"

// need to add better way to end thread pool, race conditions could cause it to never exit
// need to provide work function import for TCPServer


int main(int argc, char** argv)
{
	Server::TCPServer server(2000);

	DEBUG_PRINT("Starting server...");


	server.run();


    DEBUG_PRINT("Shutting down server...");

    return 0;
}
