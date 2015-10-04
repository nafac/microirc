#include "CommunicationConnector.hpp"
#include "GenericNetworking.hpp"
#include "GenericIRC.hpp"
#include "mirccd.hpp"
#include <stdio.h>

// client <=> client
// router
CommunicationConnector::CommunicationConnector(struct moduleconf *conf) {
	string io;
	
	if(conf->name == IRC_LIB) {
		//printf("CommunicationConnector(IRC_LIB) created..\n");

		GenericNetworking *inter_server = new GenericNetworking(CONNECT, 6669, (char *)"127.0.0.1");
		GenericIRC *irc_server = new GenericIRC(); // make these easily spawnable.

		while(1) { io = irc_server->feed(inter_server->__select(io)); }
	} else if(conf->name == IRC_CLIENT) {
		//printf("CommunicationConnector(IRC_CLIENT) created..\n");

		GenericNetworking *inter_server = new GenericNetworking(CONNECT, 6669, (char *)"127.0.0.1");	
		// Translator
		GenericNetworking *exter_server = new GenericNetworking(CONNECT, conf->port, conf->address);

		while(1) { io = exter_server->__select(inter_server->__select(io)); }
	} else if(conf->name == IRC_SERVER) {
		/*
		struct bnc_instruction_cache {
			string connection;
			string channels;
		};
		*/
		//printf("CommunicationConnector(IRC_SERVER) created..\n");

		GenericNetworking *inter_server = new GenericNetworking(CONNECT, 6669, (char *)"127.0.0.1");	// Internal	Network I/O
		// Translator
		GenericNetworking *local_server = new GenericNetworking(LISTEN, 6668, (char *)"0.0.0.0");		// External Network I/O (listen)

		while(1) { io = local_server->__select(inter_server->__select(io)); }
	}
}
