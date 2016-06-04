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
#include <sys/ioctl.h>
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
// #Alpha5r1 :: selectahz!!
// 	>> return values -1 = disconnect, 0 = nothing, 1 = write, 2 = read

// dumb fruity loops
int GenericNetworking::__server_select(int *active_fd) {
	printf("GenericNetworking::__server_select - OBSOLETED and REPLACED with better functionality @ UniversalNetwork::IPV6Server(2) \n");
	return 0;
/*
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
*/
}
//#Alpha5r1 CommunicationConnector is a static router.
// CommunicationConnector area!!
// __connector_transfer_io(source, destination) is a static router for one direction!
//#Alpha6TODO
/*
int GenericNetworking::__connector_static_router(int source, int destination) {
	string io;
	// CommunicationConnector => CommunicationHub
	if(__flush_read(source, &io) > 2) __flush_write(destination, io);
	return 0;
}
*/
// #Alpha5 :: THESE BELOW ARE READY
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
// ============================================================================================================
//#Alpha5r4
UniversalNetwork::UniversalNetwork() {
	// printf("UniversalNetwork :: init'd \n");
}
int UniversalNetwork::IPV4CreateSocket(char *address, char *port) {
	int fd;
	int s;
	struct addrinfo hints;
	// 
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family			= AF_INET;
	hints.ai_socktype		= SOCK_STREAM;
	hints.ai_flags			= AI_PASSIVE;
	hints.ai_protocol		= 0;
	hints.ai_canonname	= NULL;
	hints.ai_addr				= NULL;
	hints.ai_next				= NULL;
	// 
	s = getaddrinfo(address, port, &hints, &resolver);
	if(s != 0) {
		perror("getaddrinfo");
		exit(EXIT_FAILURE);
	}
	//
	fd = socket(resolver->ai_family, resolver->ai_socktype, resolver->ai_protocol);
	if(fd == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	//
	main_fd =	fd;
	return		fd;
}
int UniversalNetwork::IPV6CreateSocket(char *address, char *port) {
	int fd;
	int s;
	struct addrinfo hints;
	// 
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family			= AF_INET6;
	hints.ai_socktype		= SOCK_STREAM;
	hints.ai_flags			= AI_PASSIVE;
	hints.ai_protocol		= 0;
	hints.ai_canonname	= NULL;
	hints.ai_addr				= NULL;
	hints.ai_next				= NULL;
	// 
	s = getaddrinfo(address, port, &hints, &resolver);
	if(s != 0) {
		perror("getaddrinfo");
		exit(EXIT_FAILURE);
	}
	// 
	fd = socket(resolver->ai_family, resolver->ai_socktype, resolver->ai_protocol);
	if(fd == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	//
	main_fd =	fd;
	return		fd;
}
//#Alpha5r4
int UniversalNetwork::IPV6Server(char *address, char *port) {
	printf("UniversalNetwork::IPV6Server init'd \n");
	//
	int one = 1;
	// 
	int sock = IPV6CreateSocket(address, port);
	// 
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int)) < 0) perror("setsockopt");
	// 
	if(bind(sock, resolver->ai_addr, resolver->ai_addrlen) < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}
	if(listen(sock, 8) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	//
	main_fd = sock;
	return 0;
}
int UniversalNetwork::IPV4Connect(char *address, char *port) {
	printf("UniversalNetwork::IPV4Connect - Connecting %s:%s.. \n", address, port);
	// 
	int fd = IPV4CreateSocket(address, port);
	//
	if(connect(fd, resolver->ai_addr, resolver->ai_addrlen) == -1)
	{
		close(fd);
		perror("connect");
		exit(EXIT_FAILURE);
	}
	// 
	printf("UniversalNetwork::IPV4Connect - Connected %s:%s! \n", address, port);
	//
	main_fd = fd;	// object access
	return fd;		// alien access
}
int UniversalNetwork::IPV6Connect(char *address, char *port) {
	printf("UniversalNetwork::IPV6Connect - Connecting %s:%s.. \n", address, port);
	// 
	int fd = IPV6CreateSocket(address, port);
	//
	if(connect(fd, resolver->ai_addr, resolver->ai_addrlen) == -1)
	{
		close(fd);
		perror("connect");
		exit(EXIT_FAILURE);
	}
	//
	printf("UniversalNetwork::IPV6Connect - Connected %s:%s! \n", address, port);
	//
	main_fd = fd;	// object access
	return fd;		// alien access
}
/* //#Alpha5
int UniversalNetwork::__read(int fd, string *rbuf) {
	/*
	 * #Alpha4 :: Yet Another Container.
	 *	.. conclusion, C software is memory insecure, and coding secure C++ software takes time.
	 *	.. should propably just code Java or C#, but that is for pussies and I like our memories.
	 *	.. voodoo bits, woohoo.
	 * #Alpha4 :: This class is serving InternetRelayChat();
	 *	InternetRelayChat IOS = InternetRelayChat();
	 * #Alpha5 :: a non-blocking read that points a string, but not a decent coder man..
	 */
/*
	int rv = 0;
	int len = 2048;
	char buf[len];
*/
	// hyper advanced blocking detector, duh.
	/*
	if(__select_socket_state(fd) < 1)
		return 0;
	*/
/*
	// memory safety
	bzero(buf, len);
	// read
	rv = read(fd, buf, len);
	// read failed
	//	if(rv == -1) { printf("__flush_read() returned -1 => error\n\r"); return -1; }	// CRY
			if(rv == -1) return -1;																													// SIS
	// store buffer and transfer it in the chain
	*rbuf = string(buf, rv + 1);
	// debugging
	printf("__flush_read() rv=%i rbuf=%s\n\r", rv, rbuf->c_str());
	// return size in bytes
	return rv;
}
*/
string UniversalNetwork::__read(int sockfd, int *rv)
{
	if(sockfd == -1) sockfd = main_fd;
	// 
	string ret;
	char buffer[1024];
	int nbytes;
	// tweaky, tweaky
	memset(buffer, 0, 1023);
	// a blocking read
	nbytes = read (sockfd, buffer, 1024);
	if(nbytes < 0)
	{
		*rv = -1;
		perror("read");
		exit(EXIT_FAILURE);
	} else if(nbytes == 0) {
		*rv = 0;
		return("\n");
	} else {
		*rv = nbytes;
		ret = string(buffer, nbytes + 1);
		// printf("UniversalNetwork::__read - rv='%i' buf=%s \n", nbytes, ret.c_str());
		return(ret);
	}
}
// chop, chop !!
int UniversalNetwork::__write(int active_id, string buffer) {
	// 
	if(active_id < 0)
		active_id = main_fd;
	// 
	Toolbox *disposable_tools = new Toolbox();
	vector<string> cmd = disposable_tools->explode(buffer, "\n");
	// 
	int i, rv, counter = 0;
	for(i = 0; i < cmd.size(); i++)
	{
		if(cmd[i].length() < 5)
			continue;
		rv = write(active_id, cmd[i].c_str(), cmd[i].length());
		if(rv < 0)
			return -1;
		counter += rv;
	}
	return counter;
}
// I guess this is for the conditional blocks..
//	.. yes I'ma drain your CPU.
//		.. just because linux manuals are lying to me.
//#Alpha6 :: OBSOLETED
int UniversalNetwork::__select_socket_state(int target_fd) {
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
int UniversalNetwork::get_fd(void) {
	return main_fd;
}
