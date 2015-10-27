// CommunicationServer :: Text-based IPC hub.
#include "CommunicationServer.hpp"
#include "GenericNetworking.hpp"
#include <stdio.h>
#include <unistd.h>

CommunicationServer::CommunicationServer(struct moduleconf *configuration) {
	printf("CommunicationServer IPCHub initialized.\n");
	process_dispatcher();
	while(1) { printf("debug: CommunicationServer ticking.. it should not!\n\r"); sleep(30); }
}
//#Alpha5r2 :: process_dispatcher
int CommunicationServer::process_dispatcher(void) {
	// 1.0. create process dispatcher @ localhost:6669 && "DO NOT" allow GenericNetworking to server the socket.
	GenericNetworking *process_dispatcher = new GenericNetworking(LISTEN, 6669, (char *)"127.0.0.1");
	
	// 1.1. autenthicate or share time...
	//	>> DO IT LATER!
	
	// 1.2. spawn server, fingerprint it and map the connection.
	string process_dispatcher_queue;
	while(1) {
		process_dispatcher_queue = process_dispatcher->__select_transfer_io("");
		printf("process_dispatcher_queue = %s\n\r", process_dispatcher_queue.c_str());
		sleep(8);
	}
	
	// 2.0. dispatch packets.
	
	// 2.1. encrypt packets.	
	//	>> DO IT LATER!
}
