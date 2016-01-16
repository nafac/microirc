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
#include <arpa/inet.h>
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
// AWESOME !!
int GenericNetworking::IPV4Connect(char *address, char *port) {
	int fd;
	int s;
	struct addrinfo hints;
	struct addrinfo *result;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family			= AF_INET;
	hints.ai_socktype		= SOCK_STREAM;
	hints.ai_flags			= AI_PASSIVE;
	hints.ai_protocol		= 0;
	hints.ai_canonname	= NULL;
	hints.ai_addr				= NULL;
	hints.ai_next				= NULL;

	s = getaddrinfo(address, port, &hints, &result);
	if(s != 0) {
		perror("getaddrinfo");
		exit(EXIT_FAILURE);
	}

	fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if(fd == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	if(connect(fd, result->ai_addr, result->ai_addrlen) == -1) {
		close(fd);
		perror("connect");
		exit(EXIT_FAILURE);
	}

	printf("%s:%s connection alive.\n\r", address, port);

	// store dumb integers, yuck
	static_communication_router_fd = fd;	// mostly for __connector_static_router()
	clientfds.push_back(fd);		// mostly for (unused, non-functioning) select()
	return fd;				// mostly for the practice
}
int GenericNetworking::IPV6Connect(char *address, char *port) {
	// 
	CommonNetwork = new UniversalNetwork;
	int fd = CommonNetwork->IPV6CreateSocket(address, port);

	if(connect(fd, CommonNetwork->resolver->ai_addr, CommonNetwork->resolver->ai_addrlen) == -1) {
		close(fd);
		perror("connect");
		exit(EXIT_FAILURE);
	}

	printf("%s:%s connection alive.\n\r", address, port);

	// store dumb integers, yuck
	static_communication_router_fd = fd;	// mostly for __connector_static_router()
	clientfds.push_back(fd);		// mostly for (unused, non-functioning) select()
	return fd;				// mostly for the practice
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
	int liaani = select(target_fd + 1, &set, NULL, NULL, &timeout);
	//printf("__socket_state is looping on target_fd='%i' liaani='%i'\n\r", target_fd, liaani);
	return liaani;
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
		}
		*active_fd = stateful_fd;
		return rv;
	}
	*active_fd = stateful_fd;
	return rv;
}
//#Alpha5r1 CommunicationConnector is a static router.
// CommunicationConnector area!!
// __connector_transfer_io(source, destination) is a static router for one direction!
int GenericNetworking::__connector_static_router(int source, int destination) {
	string io;
	// CommunicationConnector => CommunicationHub
	if(__flush_read(source, &io) > 2) __flush_write(destination, io);
	return 0;
}
int GenericNetworking::__flush_write(int active_id, string buf) {
	// This works
	int i, rv;
	Toolbox *box = new Toolbox();
	vector<string>	commands;
	commands = box->explode(buf, "\n");
	for(i = 0; i < commands.size(); i++) {
		commands[i].append("\n");
		rv = __flush_write_sub(active_id, commands[i].c_str(), commands[i].size() + 1);
 	}
	__flush_write_sub(active_id, "\n\r<EOF>", 8);
/*
	// This is proto
	if(commands.size() > 2) {
		//printf("commands.size()=%i\n\r", commands.size());
		//rv = __flush_write_sub(active_id, buf.c_str(), buf.size() + 1);
		__flush_write_sub(active_id, "\n\r<EOF>", 8);
	}
*/
	return 0;
}
int GenericNetworking::__flush_write_sub(int active_id, const char *text, int text_size) {
	if(text_size <= 5)
		return 0;
	int rv = write(active_id, text, text_size);
	if(rv < 0)
		printf("__flush_write_sub FAIL'd :: %s\n", text);
	else
		printf("__flush_write_sub SUCC'd :: %s\n", text);
	//printf("__flush_write_sub length=%i\n", text_size);
	//memset(&text, 0, text_size -1);
	return 0;
}
// #Alpha5 :: THESE BELOW ARE READY
int GenericNetworking::__flush_read(int fd, string *rbuf) {
	/*
	 * #Alpha4 :: Yet Another Container.
	 *	.. conclusion, C software is memory insecure, and coding secure C++ software takes time.
	 *	.. should propably just code Java or C#, but that is for pussies and I like our memories.
	 *	.. voodoo bits, woohoo.
	 * #Alpha4 :: This class is serving InternetRelayChat();
	 *	InternetRelayChat IOS = InternetRelayChat();
	 */
	int rv = 0;
	int len = 2048;
	char buf[len];

	// single blocking detector
	if(__socket_state(fd) < 1)
		return 0;

	// memory safety
	bzero(buf, len);

	// read
	rv = read(fd, buf, len);
	// read failed
	if(rv == -1) { printf("__flush_read() returned -1 => error\n\r"); return -1; }

	// store buffer and transfer it in the chain
	*rbuf = string(buf, rv + 1);

	// debugging
	printf("__flush_read() rv=%i rbuf=%s\n\r", rv, rbuf->c_str());

	// return size in bytes
	return rv;
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

//#Alpha5r4
UniversalNetwork::UniversalNetwork() {
	printf("UniversalNetwork init'd \n");
}
int UniversalNetwork::IPV6CreateSocket(char *address, char *port) {
	int fd;
	int s;
	struct addrinfo hints;
	//struct addrinfo *result; // This is at UniversalNetwork->resolver.

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family			= AF_INET6;
	hints.ai_socktype		= SOCK_STREAM;
	hints.ai_flags			= AI_PASSIVE;
	hints.ai_protocol		= 0;
	hints.ai_canonname	= NULL;
	hints.ai_addr				= NULL;
	hints.ai_next				= NULL;

	s = getaddrinfo(address, port, &hints, &resolver);
	if(s != 0) {
		perror("getaddrinfo");
		exit(EXIT_FAILURE);
	}

	fd = socket(resolver->ai_family, resolver->ai_socktype, resolver->ai_protocol);
	if(fd == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
		
	return fd;
}
UniversalServer::UniversalServer(char *address, char *port) {
	printf("UniversalServer init'd \n");
	// 
	CommonNetwork = new UniversalNetwork;
	//
	int i;
	fd_set active_fdset, read_fdset;
	struct sockaddr_in clientname;
	size_t size;
	// 
	int sock = CommonNetwork->IPV6CreateSocket(address, port);
	if(bind(sock, CommonNetwork->resolver->ai_addr, CommonNetwork->resolver->ai_addrlen) < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}
	if(listen(sock, 8) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	//
	FD_ZERO(&active_fdset);
	FD_SET(sock, &active_fdset);
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
				if(i == sock) {
					//
					int newfd;
					//
					size = sizeof(clientname);
					newfd = accept(sock, (struct sockaddr *) &clientname, &size);
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
					if(read_from_client(i) < 0) {
						close(i);
						FD_CLR(i, &active_fdset);
					}
				}
			}
		}
	}
}
// unripe
int UniversalServer::read_from_client(int filedes) {
	char buffer[1024];
	int nbytes;

	nbytes = read (filedes, buffer, 1024);
	if(nbytes < 0)
	{
		perror("read");
		exit(EXIT_FAILURE);
	} else if(nbytes == 0)
		return -1;
	else {
		fprintf (stderr, "Server: got message: `%s'\n", buffer);
		return 0;
	}
}
