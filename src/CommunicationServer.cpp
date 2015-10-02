// CommunicationServer :: Text-based IPC hub.
#include "CommunicationServer.hpp"
#include "GenericNetworking.hpp"
#include <stdio.h>

CommunicationServer::CommunicationServer(struct moduleconf *configuration) {
	printf("CommunicationServer IPC Hub initialized.\n");

	// 1. listen localhost:6669 for new connections
	GenericNetworking *nio = new GenericNetworking(LISTEN, 6669, (char *)"127.0.0.1");

	// 3. one could encrypt all inter process traffic for added extra fun..

	// 2. dispatch
	
}
