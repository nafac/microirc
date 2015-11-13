#include "CommunicationConnector.hpp"
#include "GenericIRC.hpp"
#include "GenericNetworking.hpp"
#include "mirccd.hpp"
#include <stdio.h>

//#Alpha5r1 :: CommunicationConnector Point-to-Point Static Router.
//	:: It WILL only route two server modules statically, mapping and routing is done in the CommunicationServer.
//	:: client <=> client router.
CommunicationConnector::CommunicationConnector(struct moduleconf *conf) {
	// SHINY Static Router
	int static_router_one, static_router_two;
	// even older variables
	//int rv:
	//string io;

	// data transfers
	// fd_one <=> translator <=> fd_two
	
	// HOW AM I SUPPOSED TO ROUTE WITH THIS PIECE OF GARBAGE
	// .. well I don't need to as the connector module is static!!!
	
	// #Alpha5r1 :: automatize => move most crap to CommunicationServer.
	if(conf->name == IRC_LIB) {
		//printf("CommunicationConnector(IRC_LIB) created..\n");
		GenericNetworking	*interserver	= new GenericNetworking(/*CONNECT, 6669, (char *)"127.0.0.1"*/);
		GenericIRC		*irclib		= new GenericIRC(); // make these easily spawnable.

		//#Alpha5r1 :: TODO :: implement
		//while(1) { io = irc->feed(hub->__select_transfer_io(io)); }
	} else if(conf->name == IRC_CLIENT) {
		router_one = new GenericNetworking();				// CommunicationConnector is plain network router, do process and thread spawning somewhere else.
		router_one->IPV6Connect((char *)"::1", (char *)"6669");
		printf("CommunicationHub connection alive.\n\r");
		router_two = new GenericNetworking();
		router_two->IPV4Connect(conf->address, conf->port);
		printf("IRC connection alive.\n\r");
		static_route_io();						// This is the end of CommunicationConnector.
		
		//#Alpha5 :: obsolete
		//while(1) { io = ircserver->__connector_transfer_io(static_router_two, interserver->__connector_transfer_io(static_router_one, io)); }

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
//#Alpha5r1 :: Two Connections per Connector MAX!
int CommunicationConnector::static_route_io() {
	while(1) {
		router_one->__connector_static_router(router_two->static_communication_router_fd, router_one->static_communication_router_fd);
/*		// pökäle
		io = router_one->__connector_transfer_io(io);
		io = router_two->__connector_transfer_io(io);
*/
	}
	return 0;
}
