#include"websocketpp/config/asio_no_tls.hpp"
#include"websocketpp/server.hpp"
#include<iostream>

typedef websocketpp::server<websocketpp::config::asio> server;
typedef server::message_ptr message_ptr;

// define a callback to handle incoming messages
// currently echoes what was received
void do_on_message(websocketpp::connection_hdl hdl, message_ptr msg) {
	std::cout << "on_message called with hdl: " << hdl.lock().get()
		  << " and message: " << msg->get_payload()
		  << std::endl;
}

// example of how we'd use this in monoserv/monospace
// further reading: https://www.zaphoyd.com/websocketpp/manual/building-program-websocket




// in javascript land we'd have some setup like this:
// ( you should be able to run these exact commands after running ./monoserv
// function setup() = {
// 	var web_socket = new WebSocket('localhost:9002');
//	web_socket.send('start computing match + match data');
//
//	// set call back method for when we receive data
//	// then we want to update the display
//	web_socket.on_message = function(event) {
//		*update simulation with new frames received*
//	}
// 	
// }
//



// in C++ land we have some methods for dealing with calls from the client
// eg. callback for creating a new connection:
//
// void on_open(websocketpp::connection_hdl) {
// 	 spawn thread to start computing 
// }

// eg. sending frames to client
// 
// void send_some_data(void* data) {
//	try{
//		s->send(hdl, msg->get_payload(), msg->get_opcode());
//	} catch (const websocketpp::lib::error_code& e) {
//		std::cout << "Message failed: " << e
//			  << "(" << e.message() << ")" << std::endl;	
//	}
// }

// some method for tearing down socket and 
// terminating connection when simulation has finished 
// 
// void cleanup(){
// 	// close connection with client
// }

int main() {
	server _server;

	try {
		_server.set_message_handler(&do_on_message);
		_server.init_asio();
		_server.listen(9002);
		_server.start_accept();
		
		_server.run();	

	} catch (websocketpp::exception const & e) {
		std::cout << e.what() << std::endl;
	} catch (...) {
		std::cout << "other exception" << std::endl;
	}	
}
