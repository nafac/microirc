#include "CommunicationConnector.hpp"
#include "GenericNetworking.hpp"
#include "GenericIRC.hpp"
#include "mirccd.hpp"
#include <stdio.h>

// client <=> client
// router
CommunicationConnector::CommunicationConnector(struct moduleconf *conf) {
	// (obsoleted) static router
	int static_router_one, static_router_two, rv;
	// 
	string io;

	// data transfers
	// fd_one <=> translator <=> fd_two
	
	// HOW AM I SUPPOSED TO ROUTE WITH THIS PIECE OF GARBAGE
	// .. well I don't need to as the connector module is static!!!
	
	// #Alpha5r1 :: automatize => move most crap to CommunicationServer.
	if(conf->name == IRC_LIB) {
		//printf("CommunicationConnector(IRC_LIB) created..\n");
		GenericNetworking	*interserver	= new GenericNetworking(/*CONNECT, 6669, (char *)"127.0.0.1"*/);
		GenericIRC			*irclib			= new GenericIRC(); // make these easily spawnable.

		//#Alpha5r1 :: TODO :: implement
		//while(1) { io = irc->feed(hub->__select_transfer_io(io)); }
	} else if(conf->name == IRC_CLIENT) {
		//printf("CommunicationConnector(IRC_CLIENT) created..\n");
		GenericNetworking *interserver	= new GenericNetworking();	
		GenericNetworking *ircserver	= new GenericNetworking();
		static_router_one = interserver	->_connect(6669, (char *)"127.0.0.1");
		static_router_two = ircserver	->_connect(conf->port, conf->address);
		//printf("static_router_one=%i static_router_two=%i\n\r", static_router_one, static_router_two);
		while(1) { io = ircserver->__connector_transfer_io(static_router_two, interserver->__connector_transfer_io(static_router_one, io)); }

		// BUG'd => my messages are written to sock but never reach CommunicationServer..

		//while(1) { io = ircserver->__select_transfer_io(interserver->__select_transfer_io(io)); }
	} /*else if(conf->name == IRC_SERVER) {
		//printf("CommunicationConnector(IRC_SERVER) created..\n");
		GenericNetworking *intern_server = new GenericNetworking(CONNECT, 6669, (char *)"127.0.0.1");	// Internal	Network I/O
		GenericNetworking *local_server = new GenericNetworking(LISTEN, 6668, (char *)"0.0.0.0");		// External Network I/O (listen)
		while(1) {
			io = local_server->__select_transfer_io(intern_server->__select_transfer_io(io));
		}
	} */
}
