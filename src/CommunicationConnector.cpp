#include "CommunicationConnector.hpp"
#include "GenericIRC.hpp"
#include "GenericNetworking.hpp"
#include "mirccd.hpp"
#include <stdio.h>
#include <unistd.h>

/* #Alpha6 */

CommunicationConnector::CommunicationConnector()
{
	router_one = new UniversalNetwork();
	router_one->IPV6Connect((char *)"::1", (char *)"6669");
}
int CommunicationConnector::AddEndpoint(char *address, char *port)
{
	router_two = new UniversalNetwork();
	router_two->IPV4Connect(address, port);
	return 0;
}
int CommunicationConnector::__static_route_io(void)
{
	int rv = 0;
	while (1) { router_one->__write(-1, router_two->__read(-1, &rv)); }
}
int CommunicationConnector::__read()
{
	return 0;
}
int CommunicationConnector::__write()
{
	return 0;
}

/* #Alpha5 */

//#Alpha5r1 :: CommunicationConnector Point-to-Point Static Router.
//	:: It WILL only route two server modules statically, mapping and routing is done in the CommunicationServer.
//	:: client <=> client router.
/*
int CommunicationConnector::CommunicationConnector(struct moduleconf *conf) {
	// SHINY Static Router
	int static_router_one, static_router_two;
	// even older variables
	// int rv;
	// string io;

	// data transfers
	// fd_one <=> translator <=> fd_two
	
	// HOW AM I SUPPOSED TO ROUTE WITH THIS PIECE OF GARBAGE
	// .. well I don't need to as the connector module is static!!!
	
	// #Alpha6 :: 'Automated' in CommunicationServer
//	if(conf->name == IRC_LIB) {
		//printf("CommunicationConnector(IRC_LIB) created..\n");
//		GenericNetworking		*interserver	= new GenericNetworking(CONNECT, 6669, (char *)"127.0.0.1");
//		GenericIRC				*irclib				= new GenericIRC(); // make these easily spawnable.

		//#Alpha5r1 :: TODO :: implement
		//while(1) { io = irc->feed(hub->__select_transfer_io(io)); }
//	}

	if(conf->name == IRC_CLIENT) {
		//
		UniversalNetwork *HUB = new UniversalNetwork();
		HUB->IPV6Connect((char *)"::1", (char *)"6669");
		UniversalNetwork *IRC = new UniversalNetwork();
		IRC->IPV4Connect(conf->address, conf->port);
		// we do not use this here
		int rv = 0;
		while(1) {
			// router_one->__connector_static_router(router_two->static_communication_router_fd, router_one->static_communication_router_fd);
			HUB->__write(-1, IRC->__read(-1, &rv));
			// IRC->__write(HUB->__read(-1, &rv);
			//#Alpha6TODO :: copy pairing from libipc.
		}
		
		//#Alpha5r3
//		router_one = new GenericNetworking();				// CommunicationConnector is plain network router, do process and thread spawning somewhere else.
//		router_one->IPV6Connect((char *)"::1", (char *)"6669");
//		printf("CommunicationHub connection alive.\n\r");
//		router_two = new GenericNetworking();
//		router_two->IPV4Connect(conf->address, conf->port);
//		printf("IRC connection alive.\n\r");
//		static_route_io();						// This is the end of CommunicationConnector.
		
		//#Alpha5 :: obsolete
		//while(1) { io = ircserver->__connector_transfer_io(static_router_two, interserver->__connector_transfer_io(static_router_one, io)); }

		// BUG'd => my messages are written to sock but never reach CommunicationServer..

		//while(1) { io = ircserver->__select_transfer_io(interserver->__select_transfer_io(io)); }
	} else if(conf->name == IRC_SERVER) {
		//printf("CommunicationConnector(IRC_SERVER) created..\n");
		GenericNetworking *intern_server = new GenericNetworking(CONNECT, 6669, (char *)"127.0.0.1");	// Internal	Network I/O
		GenericNetworking *local_server = new GenericNetworking(LISTEN, 6668, (char *)"0.0.0.0");		// External Network I/O (listen)
		while(1) {
			io = local_server->__select_transfer_io(intern_server->__select_transfer_io(io));
		}
	}
	return 0;
}
*/
/*
//#Alpha5r1 :: bridged router I/O-thing
int CommunicationConnector::static_route_io() {
	while(1) {
		printf("CommunicationConnector::static_route_io - EMBEDDED CONNECTOR TICKING \n");
		sleep(60);
//		router_one->__connector_static_router(router_two->static_communication_router_fd, router_one->static_communication_router_fd);
//		// pökäle
//		io = router_one->__connector_transfer_io(io);
//		io = router_two->__connector_transfer_io(io);
	}
	return 0;
}
*/
