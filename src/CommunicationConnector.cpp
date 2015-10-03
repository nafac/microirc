#include "CommunicationConnector.hpp"
#include "GenericNetworking.hpp"
#include "GenericIRC.hpp"
#include "mirccd.hpp"
#include <stdio.h>

CommunicationConnector::CommunicationConnector(struct moduleconf *conf) {
	string io;
	if(conf->name == IRC_LIB) {
		printf("CommunicationConnector(IRC_LIB) created.. (action=%i)\n", conf->name);
		GenericNetworking	*iio = new GenericNetworking(CONNECT, 6669, (char *)"127.0.0.1");	// Internal Network I/O
		GenericIRC			*irc = new GenericIRC(void);													// please arrange one GenericIRC per server + one for each channel ..
																														// .. it could be configurable too.
		while(1) {
			io = "\n\r";									// .. you say what?
			io = irc->feed(iio->__select(io));		// interserver is at port 6669 anyway ..
		}
		// make these easily spawnable.
	} else if(conf->name == IRC_CLIENT) {
		printf("CommunicationConnector(IRC_CLIENT) created.. (action=%i)\n", conf->name);
		GenericNetworking *iio = new GenericNetworking(CONNECT, 6669, (char *)"127.0.0.1");	// Internal Network I/O
		GenericNetworking *eio = new GenericNetworking(CONNECT, conf->port, conf->address);	// External Network I/O (connect)

		while(1) {
			io = "\n\r";									// .. you say what?
			io = iio->__select(eio->__select(io));	// interserver is at port 6669 anyway ..
		}
/*
		while(1) {
			eio->__select();
			//eio->__read();
		}
*/
		// Can it has fd? no it can not.
		// Implement routing.
	} else if(conf->name == IRC_SERVER) {
		printf("CommunicationConnector(IRC_SERVER) created.. (action=%i)\n", conf->name);
		GenericNetworking *iio = new GenericNetworking(CONNECT, 6669, (char *)"127.0.0.1");	// Internal	Network I/O
		GenericNetworking *eio = new GenericNetworking(LISTEN, 6668, (char *)"0.0.0.0");		// External Network I/O (listen)
		// just copy it above, I do 2 things at once.
	}
}
