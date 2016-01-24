// CommunicationServer :: Text-based IPC hub.
#include "CommunicationServer.hpp"
#include "GenericNetworking.hpp"
#include "Toolbox.hpp"
#include "modules/mod_irc.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/time.h>
#include <pthread.h>

// #include <iostream>
// using namespace std;

// CommunicationHub is a single, managed thread !!
CommunicationServer::CommunicationServer() {
	//#Alpha5r3
	//process_dispatcher();
	//#Alpha5r4
	UniversalNetwork *HUB = new UniversalNetwork();
	HUB->IPV6Server((char *)"::1", (char *)"6669");
	//
	int i, rv;
	fd_set active_fdset, read_fdset;
	struct sockaddr_in clientname;
	size_t size;
	//
	string reader;
	Toolbox *disposable_tools = new Toolbox();
	//
	int fd = HUB->get_fd();
	// 
	FD_ZERO(&active_fdset);
	FD_SET(fd, &active_fdset);
	// 
	while(1) {
		read_fdset = active_fdset;
		// block
		if(select(FD_SETSIZE, &read_fdset, NULL, NULL, NULL) < 0) {
			perror("select");
			exit(EXIT_FAILURE);
		}
		// service
		for(i = 0; i < FD_SETSIZE; ++i) {
			if(FD_ISSET(i, &read_fdset)) {
				// accept new clients from listener
				if(i == fd) {
					//
					int newfd;
					//
					size = sizeof(clientname);
					newfd = accept(fd, (struct sockaddr *) &clientname, &size);
					if(newfd < 0) {
						perror("accept");
						exit(EXIT_FAILURE); // dont
					}
					fprintf(stderr, "Server: connect from host %s:%hd. \n",
						inet_ntoa(clientname.sin_addr),
						ntohs(clientname.sin_port));
					FD_SET(newfd, &active_fdset);
				} else {
					// data arriving from already-connected socket
					reader = HUB->__read(i, &rv);
					if (reader.length() < 5)
						continue;
					// IPC-Hub
					if (disposable_tools->Contains(reader, "NOTICE AUTH"))
					{
						// printf("NOTICE AUTH DETECTED\n\r");
						//#Alpha6TODO :: Implement ThreadDispatcher.
						//#Alpha6TODO :: Add module hooks.
						// 
						mod_irc *module_irc = new mod_irc();
						//#Alpha6 :: do not close listener
						// close(i);
						// FD_CLR(i, &active_fdset);
						// continue;
					}
					printf("CommunicationServer::CommunicationServer :: reader=%s\n\r", reader.c_str());
					// Feed(HUB->__read(i, &rv));
					if(rv < 0) {
						close(i);
						FD_CLR(i, &active_fdset);
					}
				}
			}
		}
	}
	//while(1) { printf("UniversalNetwork::IPV6Server (CommunicationServer::Hub) ticks, it should not !! \n"); sleep(30); }	// not anymore
	//printf("C++ CommunicationServer is dead and buried, skipped !!\n\r");																									//
}
int CommunicationServer::Feed(string reader)
{
	//#Alpha6
	Toolbox *disposable_tools = new Toolbox();
	vector<string> feed = disposable_tools->explode(reader, "\n");
	// 
	for(int i = 0; i < feed.size(); i++)
	{
		if(feed[i].length() < 5)
			continue;
		// protocol
		if(disposable_tools->Contains(feed[i], "NOTICE AUTH")) {
			printf("CommunicationServer::Feed NOTICE AUTH DETECTED\n\r");
		}
		printf("CommunicationServer::Feed feed[%i]=%s \n\r", i, feed[i].c_str());
 	}
}
//#Alpha5r2 :: process_dispatcher
/*
int CommunicationServer::_process_dispatcher_loop(void) {
	// 1.0. create process dispatcher @ localhost:6669 && "DO NOT" allow GenericNetworking to server the socket.
	process_dispatcher = new GenericNetworking();
	int process_dispatcher_fd = process_dispatcher->_listen(6669, (char *)"127.0.0.1");
	if(process_dispatcher_fd < 0) {
		printf("fatal error: process_dispatcher() failed to initialize!\n\r");
		exit(-1);
	}
	printf("process_dispathcer up and running.\n\r");
	
	// 1.1. autenthicate or share time...
	//	>> DO IT LATER!
	
	// 1.2. spawn server, fingerprint it and map the connection. One could do some brain simulation widit.
	int fd, rv;
	string process_dispatcher_queue;
	while(1) {
*/
/*		// pökäle
		fd = 0, rv = 0;
		rv = process_dispatcher->__server_select(&fd);
		printf("process_dispatcher(): fd=%i rv=%i\n\r", fd, rv);
		if(rv > 0) {
			// A
			process_dispatcher_queue = process_dispatcher->__connector_transfer_io(fd, process_dispatcher_queue);
			process_dispatcher_queue = _process_dispatcher_read(fd);
			// B
			printf("process_dispatcher_queue = '%s' \n\r", process_dispatcher_queue.c_str());
		}
		// __select_active return values -1 = disconnect, 0 = nothing, 1 = write, 2 = read
		//int GenericNetworking::__select_active_state(int *active_fd);
		//printf("process_dispatcher(): fd=%i rv=%i\n\r", fd, rv);

		//process_dispatcher_queue = process_dispatcher->__select_transfer_io("");
*/
/*
		rv = 0;
		fd = 0;
		//rv = process_dispatcher->__server_select(&fd);
		if(rv < 0) {
			// disconnect
		} else if(rv == 0) {
			// ping pong
		} else if(rv > 0) {
			// read
			//process_dispatcher_queue = _process_dispatcher_read(fd);
			//printf("process_dispatcher_queue = %s", process_dispatcher_queue.c_str());
		}
*/
/*
		process_dispatcher_queue = _process_dispatcher_read(process_dispatcher_fd);
		if(process_dispatcher_queue != "NULL") {
			printf("process_dispatcher_queue = %s\n\r", process_dispatcher_queue.c_str());
		}
		sleep(0.1);
*/
/*
	}
	
	// 2.0. dispatch packets.
	
	// 2.1. encrypt packets.	
	//	>> DO IT LATER!
} */
//#Alpha5r3 :: This will propably be ProcessDispatcher or another dumb complexity container.
//#Alpha5r4 :: This is old, non-functioning code. do not use !!
int		sock;
int		connectlist[1024];
fd_set	socks;
int		highsock;
int CommunicationServer::process_dispatcher(void) {
	// 
	int reuse_addr = 1, reuse_port = 1;
	// 
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		perror("socket");
		exit(-1);
	}
	// 
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr));
	setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &reuse_port, sizeof(reuse_port));
	setnonblocking(sock);
	// 
	struct sockaddr_in server_address;
	memset((char *) &server_address, 0, sizeof(server_address));
	server_address.sin_family			= AF_INET;
	server_address.sin_addr.s_addr	= htonl(INADDR_ANY);
	server_address.sin_port				= 6669;
	//
	if(bind(sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
		perror("bind");
		close(sock);
		exit(-1);
	}
	//
	listen(sock, 1024);
	printf("process_dispatcher() up and running.\n\r");
	int highsock = sock;
	//
	memset((char *) &connectlist, 0, sizeof(connectlist));
	// 
	struct timeval timeout;
	int readsocks;
	while(1) {
		handle_new_connection();
		// 
		build_select_list();
		// 
		timeout.tv_sec = 0;
		timeout.tv_usec = 32;
		// 
		readsocks = select(highsock + 1, &socks, (fd_set *) 0, (fd_set *) 0, &timeout);
		// 
		if(readsocks < 0) {
			perror("select");
			continue;
		}
		if(readsocks == 0) {
			//printf("process_dispatcher() ticking ..\n\r");
			// write
			//handle_new_connection();
		} else {
			printf("select returned > 0\n\r");
			read_socks();
		}
	}
	return 0;
}
void CommunicationServer::build_select_list(void) {
		int listnum;
		//printf("building listener fd=%d\n\r", sock);
		FD_ZERO(&socks);
		FD_SET(sock, &socks);
		for(listnum = 0; listnum < 1024; listnum++) {
			if(connectlist[listnum] != 0) {
				printf("building fd=%d\n\r", connectlist[listnum]);
				FD_SET(connectlist[listnum], &socks);
				if(connectlist[listnum] > highsock)
					highsock = connectlist[listnum];
			}
		}
		return;
}
void CommunicationServer::read_socks(void) {
	int listnum;
	if(FD_ISSET(sock, &socks)) {
		printf("accepting..\n\r");
		handle_new_connection();
	}
	for(listnum = 0; listnum < 1024; listnum++) {
		if(FD_ISSET(connectlist[listnum], &socks))
			deal_with_data(listnum);
	}
	return;
}
void CommunicationServer::handle_new_connection(void) {
	int listnum;
	int connection;
	//
	connection = accept(sock, NULL, NULL);
	if(connection < 0) {
		//perror("accept");
		return;
	}
	//
	setnonblocking(connection);
	//
	for(listnum = 0; (listnum < 1024) && (connection != -1); listnum++) {
		if(connectlist[listnum] == 0) {
			printf("handle_new_connection(): connection accepted FD=%d; Slot=%d\n\r", connection, listnum);
			connectlist[listnum] = connection;
			connection = -1;
		}
	}
	if(connection != -1) {
		printf("no room left for a new client\n\r");
		//sock_puts(connection, "höhö");
		close(connection);
	}
	return;
}
void CommunicationServer::deal_with_data(int listnum) {
	printf("reading..");
	return;
}
void CommunicationServer::setnonblocking(int sock)
{
	int opts;
	opts = fcntl(sock, F_SETFL);
	if(opts < 0) {
		perror("fcntl(F_SETFL)");
		exit(EXIT_FAILURE);
	}
	opts = (opts | O_NONBLOCK);
	if(fcntl(sock, F_SETFL, opts) < 0) {
		perror("fcntl(F_SETFL)");
		exit(EXIT_FAILURE);
	}
	return;
}

int CommunicationServer::__process_dispatcher(void) {
	//int myfds[1024]
	int maxfd, j;
	int result;
	fd_set readset;
	// !!
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 32;
	// ...
	FD_ZERO(&readset);
	maxfd = 0;
	for(j = 0; j < 1024; j++) {
		FD_SET(connectlist[j], &readset);
		maxfd = (maxfd > connectlist[j]) ? maxfd:connectlist[j];
	}
	// ...
	result = select(maxfd + 1, &readset, NULL, NULL, &timeout);
	if(result == -1) {
		printf("select::disconnect please debug!!\n\r");
	} else {
		for(j = 0; j < 1024; j++) {
			if(FD_ISSET(connectlist[j], &readset)) {
				printf("select::read debug!!\n\r");
			}
		}
	}
	return 0;
}

/*
	struct timeval timeout;
	int readsocks;
	while(1) {
		build_select_list();
		// 
		timeout.tv_sec = 0;
		timeout.tv_usec = 32;
		// 
		readsocks = select(highsock + 1, &socks, (fd_set *) 0, (fd_set *) 0, &timeout);
		// 
		if(readsocks < 0) {
			perror("select");
			continue;
		}
		if(readsocks == 0) {
			//printf("process_dispatcher() ticking ..\n\r");
			// write
			//handle_new_connection();
		} else {
			printf("select returned > 0\n\r");
			read_socks();
		}
	}
*/
