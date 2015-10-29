// CommunicationServer :: Text-based IPC hub.
#include "CommunicationServer.hpp"
#include "GenericNetworking.hpp"
#include <stdio.h>
#include <unistd.h>

CommunicationServer::CommunicationServer(struct moduleconf *configuration) {
	process_dispatcher();
	while(1) { printf("debug: CommunicationServer ticking.. it should not!\n\r"); sleep(30); }
}
//#Alpha5r2 :: process_dispatcher
int CommunicationServer::process_dispatcher(void) {
	// 1.0. create process dispatcher @ localhost:6669 && "DO NOT" allow GenericNetworking to server the socket.
	GenericNetworking *process_dispatcher = new GenericNetworking();
	if(process_dispatcher->_listen(6669, (char *)"127.0.0.1") < 0) { printf("fatal error: process_dispatcher() failed to initialize!\n\r"); exit(-1); }
	printf("process_dispathcer up and running.\n\r");
	
	// 1.1. autenthicate or share time...
	//	>> DO IT LATER!
	
	// 1.2. spawn server, fingerprint it and map the connection. One could do some brain simulation widit.
	int fd, rv;
	string process_dispatcher_queue;
	while(1) {
		fd = 0;
		rv = process_dispatcher->__server_select(&fd);
		printf("process_dispatcher(): fd=%i rv=%i\n\r", fd, rv);
		if(rv > 0) {
			process_dispatcher_queue = process_dispatcher->__connector_transfer_io(fd, "");
			printf("process_dispatcher_queue = '%s' \n\r", process_dispatcher_queue.c_str());
		}
		sleep(1);
		// __select_active return values -1 = disconnect, 0 = nothing, 1 = write, 2 = read
		//int GenericNetworking::__select_active_state(int *active_fd);
		//printf("process_dispatcher(): fd=%i rv=%i\n\r", fd, rv);

//		process_dispatcher_queue = process_dispatcher->__select_transfer_io("");
//		printf("process_dispatcher_queue = %s\n\r", process_dispatcher_queue.c_str());
		//sleep(8);
	}
	
	// 2.0. dispatch packets.
	
	// 2.1. encrypt packets.	
	//	>> DO IT LATER!
}
