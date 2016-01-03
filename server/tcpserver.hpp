/*
 * tcpserver.hpp
 *
 *  Created on: Dec 31, 2015
 *      Author: Zamaster
 */

#ifndef TCPSERVER_HPP_
#define TCPSERVER_HPP_
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace Server
{
    class TCPServer
    {
    	typedef unsigned short Port;
    public:
    	TCPServer();
    	TCPServer(Port _port);

    	void setPort(Port _port);
    	void run();
    private:
    	void construct();
        static void taskMain(tcp::socket& _socket);
        static void socketDispatcher(tcp::socket* _socketPtr);
    	Port port;
    };

}


#endif /* TCPSERVER_HPP_ */
