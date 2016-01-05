/*
 * main.cpp
 *
 *  Created on: Dec 31, 2015
 *      Author: ubuntu
 */
#include "debug.hpp"
#include "server/server.hpp"

using namespace Monospace;

void sillyTask(Server::Socket& _sock, Server::TCPServerAccess& _access)
{

}

int main(int argc, char** argv)
{
	Monospace::Server::TCPServer server(2000, sillyTask);

	DEBUG_PRINT("Starting server...");

	server.run();

	return 0;
}
