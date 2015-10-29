// #Alpha5 :: Chaos Communication Server 2015
// ..it basically just connects everything to the main "hub".
#include "GenericNetworking.hpp"
#include "Toolbox.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>

// #Alpha5 :: GenericNetworking
GenericNetworking::GenericNetworking(/*position _mode, int port, char *address*/) {
/*
	// WTF?!
	if(_mode == CONNECT)
		_connect(port, address);
	if(_mode == LISTEN)
		_listen(port, address);
*/
}
// #Alpha5 :: connection management
int GenericNetworking::_connect(int port, char *address) {
	int fd;
	// This function does not block, is good.
	struct hostent			*resolver;
	struct sockaddr_in	serv_addr;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0) {
		printf("%i: failed to create socket\n", __LINE__);
		return 1;
	}

	resolver = gethostbyname(address);
	if(resolver == NULL) {
		printf("Failed to get resolver.\n");
		return 1;
	}

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family	= AF_INET;
	bcopy((char *)resolver->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, resolver->h_length);
	serv_addr.sin_port	= htons(port);

	if(connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("Failed to connect.\n");
		return 1;
	}

	printf("Connected @ %s:%i\n", address, port);
	
	//#Alpha5r3
	//static_connector_router = fd;
	clientfds.push_back(fd);
	
	//#Alpha5r1
	return fd;
	
	//#Alpha5r1 return data, loop it through a generic "interface".
}
int GenericNetworking::_listen(int port, char *address) {
	int fd;
	int newsockfd;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0) {
		printf("%i: failed to create socket\n", __LINE__);
		return -1;
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= htonl(INADDR_LOOPBACK);
	serv_addr.sin_port			= htons(port);

	int yes = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes));
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

	if(bind(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("%i: failed to bind socket\n", __LINE__);
		return -1;
	}

	listener_id = fd;
	clientfds.push_back(listener_id);
	listen(listener_id, 8);
	printf("Listener alive @ %s:%i\n", address, port);

	return 0;
}
// #Alpha5r1 :: selectahz!!
// 	>> return values -1 = disconnect, 0 = nothing, 1 = write, 2 = read

// I guess this is for the conditional blocks..
//	.. yes I'ma drain your CPU.
//		.. Just because linux manuals are lying to me.
int GenericNetworking::__socket_state(int target_fd) {
	//#Alpha5
	struct timeval timeout;
	timeout.tv_sec	= 0;
	timeout.tv_usec	= 32;
	fd_set set;
	// Linux manuals are lying to me!
	FD_ZERO(&set);
	FD_SET(target_fd, &set);
	// nafac.co stateful blocks
	return select(target_fd + 1, &set, NULL, NULL, &timeout);
}
// dumb fruity loops
int GenericNetworking::__server_select(int *active_fd) {
	// DONT!
	//if(active_fd != 0) return 0;
	//#Alpha5r2
	int n = 0, rv = 0, stateful_fd = -1;
	
	for(n = 0; n <= clientfds.size(); n++) {
		stateful_fd = clientfds[n];
		// .. play hangman @ hamppu.net, always guess letter "H" = WIN!
		rv = __socket_state(stateful_fd);
		printf("__server_select is looping on stateful_fd='%i' rv='%i'\n\r", stateful_fd, rv);

		if(rv == -1) {
			close(stateful_fd);
			continue;
		} else {
			if(stateful_fd == listener_id) {
				__accept();
				continue;
			}
			*active_fd = stateful_fd;
			return rv;
		}
	}
	*active_fd = stateful_fd;
	return -1;
}
//#Alpha5r1 CommunicationConnector is a static router.
string GenericNetworking::__connector_transfer_io(int static_connector_router, string io) {
	__flush_write(static_connector_router, io);
	return __flush_read(static_connector_router);
}
int GenericNetworking::__flush_write(int active_id, string buf) {
	int i, rv;
	Toolbox *box = new Toolbox();
	vector<string>	commands;
	commands = box->explode(buf, "\n");
	for(i = 0; i < commands.size(); i++)
		rv = __flush_write_sub(active_id, commands[i].c_str(), commands[i].size() + 1);
	return 0;
}
int GenericNetworking::__flush_write_sub(int active_id, const char *text, int text_size) {
	if(text_size <= 3)
		return 0;
	int rv = write(active_id, text, text_size);
	if(rv < 0)
		printf("__flush_write_sub FAIL'd :: %s\n", text);
	else
		printf("__flush_write_sub SUCC'd :: %s\n", text);
	//printf("__flush_write_sub length=%i\n", text_size);
	return 0;
}
// #Alpha5 :: THESE BELOW ARE READY
string GenericNetworking::__flush_read(int active_id) {
	// #Alpha4 :: Yet Another Container.
	//	.. conclusion, C software is memory insecure, and coding secure C++ software takes time.
	//	.. should propably just code Java or C#, but that is for pussies and I like our memories.
	//	.. voodoo bits, woohoo.
	// #Alpha4 :: This class is serving InternetRelayChat();
	//InternetRelayChat IOS = InternetRelayChat();
	// my reads are always non-blocking
	if(__socket_state(active_id) < 1)
		return "\n\r";
	int len = 2048;
	char buf[len];
	string commands;
	bzero(buf, len);
	read(active_id, buf, len);
	commands = string(buf, len + 1);
	printf("__flush_read() return value='%s'\n\r", commands.c_str());
	return commands;
}
int GenericNetworking::__accept() {
	// Store client data please, I like voodoo bits.
	socklen_t clilen;
	struct sockaddr_in cli_addr;
	clilen = sizeof(cli_addr);
	// Things happen..
	int newsockfd = accept(listener_id, (struct sockaddr *)&cli_addr, &clilen);
	if(newsockfd < 0) {
		printf("GenericNetworking::__accept(): failed!\n\r");
		return -1;
	}
	printf("Adding newsockfd=%i to clientfds\n\r", newsockfd);
	clientfds.push_back(newsockfd);
	return newsockfd;
}
