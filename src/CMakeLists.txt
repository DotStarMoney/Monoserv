cmake_minimum_required(VERSION 3.2)
project(Monoserv)


set(SRC_FILES 
	
	main.cpp 
	threadpool.cpp
	tcpserver.cpp
	PARENT_SCOPE)

source_group("Source Files" FILES ${SRC_FILES})
