/*
 * tcpserveraccess.hpp
 *
 *  Created on: Jan 4, 2016
 *      Author: ubuntu
 */

#ifndef SOCKETACCESS_HPP_
#define SOCKETACCESS_HPP_

#include <boost/asio.hpp>

class TCPServer;

using boost::asio::ip::tcp;

namespace Monospace
{
	namespace Server
	{
		class SocketAccess
		{
			friend class TCPServer;
		public:
			SocketAccess(tcp::socket* _socket)
			{
				socket = _socket;
				sockClosed = false;
				lastError.clear();
			}
			size_t readData(char* _data, size_t _length)
			{
				size_t recLength;
				recLength = socket->read_some(boost::asio::buffer(_data, _length), lastError);
				if(lastError) sockClosed = true;
				return recLength;
			}
			void writeData(char* _data, size_t _length)
			{
				if(!sockClosed) boost::asio::write(*socket, boost::asio::buffer(_data, _length));
			}
			bool isClosed()
			{
				return sockClosed;
			}

			boost::system::error_code getError()
			{
				return lastError;
			}

		private:
			bool sockClosed;
			tcp::socket* socket;
			boost::system::error_code lastError;
			void deleteSocket()
			{
				delete socket;
			}
		};
	}
}

#endif /* SOCKETACCESS_HPP_ */
