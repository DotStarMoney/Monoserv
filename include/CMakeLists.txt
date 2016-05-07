cmake_minimum_required(VERSION 3.2)
project(Monoserv)


set(HDR_FILES 
	
	threadpool.hpp
	debug.hpp
	server.hpp
	socketaccess.hpp
	tcpserver.hpp
	
	PARENT_SCOPE)

source_group("Header Files" FILES ${HDR_FILES})
