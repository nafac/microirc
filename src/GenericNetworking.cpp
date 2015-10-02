// #Alpha5 :: Chaos Communication Server 2015
// ..it basically just connects everything to the main "hub".
#include "GenericNetworking.hpp"
// yuck
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>

// #Alpha5 :: GenericNetworking
GenericNetworking::GenericNetworking(position _mode, int port, char *address) {
	// do not give a chance to manipulate it after creation
	if(_mode == CONNECT)
		_connect(port, address);
	if(_mode == LISTEN)
		_listen(port, address);
}
// #Alpha5 :: private functions
// #Alpha5 :: This function does not block, is good.
int GenericNetworking::_connect(int port, char *address) {
	struct hostent			*resolver;
	struct sockaddr_in	serv_addr;

	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(_fd < 0) {
		printf("%i: failed to create socket\n", __LINE__);
		return 1;
	}

	resolver = gethostbyname(address);
	if(resolver == NULL) {
		cout << "failed to retrieve resolver!!" << endl;
		return 1;
	}

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family	= AF_INET;
	bcopy((char *)resolver->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, resolver->h_length);
	serv_addr.sin_port	= htons(port);

	if(connect(_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		cout << "failed to connect!!" << endl;
		return 1;
	}
	//pthread_create(threadpool[0], NULL, _write, NULL);
	//pthread_create(threadpool[1], NULL, _read, NULL);
	printf("Connected @ %s:%i\n", address, port);
	return 0;
}
// #Alpha5TODO :: This function blocks, it should not.
int GenericNetworking::_listen(int port, char *address) {
	int newsockfd;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;

	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(_fd < 0) {
		printf("%i: failed to create socket\n", __LINE__);
		return 1;
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);	// #Alpha5 TODO :: only accept local connections
	serv_addr.sin_port = htons(port);

	int yes = 1;
	setsockopt(_fd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes));
	setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

	if(bind(_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("%i: failed to bind socket\n", __LINE__);
		return 1;
	}

	//
	char buffer[256];
	int n;

	printf("Listener alive @ %s:%i\n", address, port);

	while(1) {
		printf("ticking..\n");
		listen(_fd, 8);
		clilen = sizeof(cli_addr);
		newsockfd = accept(_fd, (struct sockaddr *)&cli_addr, &clilen); // #Alpha5 TODO :: do arrays
		if(newsockfd < 0) {
			printf("%i: failed to accept socket\n", __LINE__);
			continue;
		}

		//
		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		if(n < 0) {
			printf("error reading on socket.. closing..\n");
			close(newsockfd);
			continue;
		}
		n = write(newsockfd, "I got your message", 18);
		if(n < 0) {
			printf("error writing on socket.. closing..\n");
			close(newsockfd);
			continue;
		}
	}
	close(newsockfd);
	close(_fd);
	printf("LISTEN FAILED\n");
	return 0;
}
