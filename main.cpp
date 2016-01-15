/*
 * main.cpp
 *
 *  Created on: Dec 31, 2015
 *      Author: ubuntu
 */
#include "debug.hpp"
#include "server/server.hpp"

using namespace Monospace;

void sillyTask(Server::SocketAccess& _access)
{
	char data[1024];
	size_t length;

	while(!_access.isClosed())
	{
		length = _access.readData(data, 1024);
		data[1023] = 0;
		DEBUG_PRINT("%s\n", data);
		_access.writeData(data, length);
	}
}

int main(int argc, char** argv)
{
	Server::TCPServer server(2000, sillyTask);

	DEBUG_PRINT("Starting server...");

	server.run();

	return 0;
}
