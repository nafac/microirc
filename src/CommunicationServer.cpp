// CommunicationServer :: Text-based IPC hub.
#include "CommunicationServer.hpp"
#include "GenericNetworking.hpp"
#include <stdio.h>
#include <unistd.h>

CommunicationServer::CommunicationServer(struct moduleconf *configuration) {
	printf("CommunicationServer IPCHub initialized.\n");

	// 1. listen localhost:6669 for new connections
	// #Alpha5r1 :: allow GenericNetworking serve the socket
	GenericNetworking *nio = new GenericNetworking(LISTEN, 6669, (char *)"127.0.0.1");

	// 3. one could encrypt all inter process traffic for added extra fun..

	// 2. dispatch
	while(1) { printf("debug: CommunicationServer ticking.. it should not..\n"); sleep(30); }
}

