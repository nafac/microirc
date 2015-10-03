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
GenericNetworking::GenericNetworking(position _mode, int port, char *address) {
	// do not give a chance to manipulate it after creation
	if(_mode == CONNECT)
		_connect(port, address);
	if(_mode == LISTEN)
		_listen(port, address);
}
// #Alpha5 :: this is awesome
string GenericNetworking::__select(string io) {
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 32;

	fd_set set;
	FD_ZERO(&set);
	FD_SET(_fd, &set);
	
	int rv = select(_fd + 1, &set, NULL, NULL, &timeout);
	if(rv == -1) {
		printf("select(%i) returned -1, closing socket. I should propably reconnect ASAP..\n", _fd);
		close(_fd); // #Alpha5TODO :: reconnect
		return -1;
	} else if(rv == 0) {
		__flush_write(&io);
		return 0;
	} else if(rv > 0) {
		printf("select(%i) returned >1, reading ..\n", _fd);
		__read();
		return 1;
	}
	return 0; // ahh, but this is important.
}
// #Alpha5 :: private area
string GenericNetworking::__read(void) {
	// #Alpha4 :: Yet Another Container.
	//	.. conclusion, C software is memory insecure, and coding secure C++ software takes time.
	//	.. should propably just code Java or C#, but that is for pussies and I like our memories.
	//	.. voodoo bits, woohoo.
	// #Alpha4 :: This class is serving InternetRelayChat();
	//InternetRelayChat IOS = InternetRelayChat();
	int len = 2048;
	char buf[len];
	string commands;

	bzero(buf, len);
	read(_fd, buf, len);
	commands = string(buf, len + 1);

	printf("__read() returned %s\n", buf);
	return commands;
}
int GenericNetworking::__flush_write(string *write_buffer) {
	Toolbox *box = new Toolbox();
	vector<string> write_buffer_list;
	int i;
	// split lines
	write_buffer_list = box->explode(write_buffer, "\n");
	// write everything to socket, at instant.
	for(i = 0; i < write_buffer_list.size(); i++) {
		if(write(_fd, "PONG\n\r", 7) < 0)
			printf("Failed to write PONG to fd:%i\n", _fd);
		else
			printf("Successfully wrote PONG to fd:%i\n", _fd);
	}
	// reset I/O pointer
	write_buffer = (string)"\n\r";
	return 0;
}
int GenericNetworking::_connect(int port, char *address) {
	// This function does not block, is good.
	struct hostent			*resolver;
	struct sockaddr_in	serv_addr;

	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(_fd < 0) {
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

	if(connect(_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("Failed to connect.\n");
		return 1;
	}
	//pthread_create(threadpool[0], NULL, _write, NULL);
	//pthread_create(threadpool[1], NULL, _read, NULL);
	printf("Connected @ %s:%i\n", address, port);
	return 0;
}
// #Alpha5TODO :: This function blocks, it should not.
int GenericNetworking::_listen(int port, char *address) {
	// This function does block, no good.
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
