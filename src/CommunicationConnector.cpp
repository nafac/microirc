#include "CommunicationConnector.hpp"
#include "GenericNetworking.hpp"
#include "GenericIRC.hpp"
#include "mirccd.hpp"
#include <stdio.h>

// client <=> client
// router
CommunicationConnector::CommunicationConnector(struct moduleconf *conf) {
	string io;
	
	// #Alpha5r1 :: automatize
	if(conf->name == IRC_LIB) {
		//printf("CommunicationConnector(IRC_LIB) created..\n");

		GenericNetworking	*hub = new GenericNetworking(CONNECT, 6669, (char *)"127.0.0.1");
		GenericIRC			*irc = new GenericIRC(); // make these easily spawnable.

		while(1) { io = irc->feed(hub->__select(io)); }
	} else if(conf->name == IRC_CLIENT) {
		//printf("CommunicationConnector(IRC_CLIENT) created..\n");

		GenericNetworking *intern_server = new GenericNetworking(CONNECT, 6669, (char *)"127.0.0.1");	
		// Translator
		GenericNetworking *extern_server = new GenericNetworking(CONNECT, conf->port, conf->address);

		while(1) { io = extern_server->__select(intern_server->__select(io)); }
	} else if(conf->name == IRC_SERVER) {
		/*
		struct bnc_instruction_cache {
			string connection;
			string channels;
		};
		*/
		//printf("CommunicationConnector(IRC_SERVER) created..\n");

		GenericNetworking *intern_server = new GenericNetworking(CONNECT, 6669, (char *)"127.0.0.1");	// Internal	Network I/O
		// Translator
		GenericNetworking *local_server = new GenericNetworking(LISTEN, 6668, (char *)"0.0.0.0");		// External Network I/O (listen)

		while(1) { io = local_server->__select(intern_server->__select(io)); }
	}
}
