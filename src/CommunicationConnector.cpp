#include "CommunicationConnector.hpp"
#include "GenericNetworking.hpp"
#include "mirccd.hpp"
#include <stdio.h>

CommunicationConnector::CommunicationConnector(struct moduleconf *conf) {
	if(conf->name == IRC_LIB) {
		printf("CommunicationConnector(IRC_LIB) created.. (action=%i)\n", conf->name);
		// create one or more copies of the IRC_LIB and serve it.
		// > option 1. use one copy of libirc per daemon
		// > option 2. use one copy of libirc per server  
		// > option 3. use one copy of libirc per channel <--!
		// > option 4. use one copy of libirc per user
	} else if(conf->name == IRC_CLIENT) {
		printf("CommunicationConnector(IRC_CLIENT) created.. (action=%i)\n", conf->name);
		GenericNetworking *iio = new GenericNetworking(CONNECT, 6669, (char *)"127.0.0.1");	// Internal Network I/O
		GenericNetworking *eio = new GenericNetworking(CONNECT, conf->port, conf->address);	// External Network I/O (connect)
	} else if(conf->name == IRC_SERVER) {
		printf("CommunicationConnector(IRC_SERVER) created.. (action=%i)\n", conf->name);
		GenericNetworking *iio = new GenericNetworking(CONNECT, 6669, (char *)"127.0.0.1");	// Internal	Network I/O
		GenericNetworking *eio = new GenericNetworking(LISTEN, 6668, (char *)"0.0.0.0");		// External Network I/O (listen)
	}
}
