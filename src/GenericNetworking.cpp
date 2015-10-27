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
// #Alpha5 :: connection management
int GenericNetworking::_connect(int port, char *address) {
	// This function does not block, is good.
	struct hostent			*resolver;
	struct sockaddr_in	serv_addr;

	active_id = socket(AF_INET, SOCK_STREAM, 0);
	if(active_id < 0) {
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

	if(connect(active_id, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("Failed to connect.\n");
		return 1;
	}

	printf("Connected @ %s:%i\n", address, port);
	
	clientfds.push_back(active_id);
	
	return 0;
	
	//#Alpha5r1 return data, loop it through a generic "interface".
}
// #Alpha5 :: this is not a generic function, it only serves the hub
int GenericNetworking::_listen(int port, char *address) {
	// This function does block, no good.
	int newsockfd;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;

	active_id = socket(AF_INET, SOCK_STREAM, 0);
	if(active_id < 0) {
		printf("%i: failed to create socket\n", __LINE__);
		return 1;
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= htonl(INADDR_LOOPBACK);
	serv_addr.sin_port			= htons(port);

	int yes = 1;
	setsockopt(active_id, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes));
	setsockopt(active_id, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

	if(bind(active_id, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("%i: failed to bind socket\n", __LINE__);
		return 1;
	}

	listener_id = active_id;
	clientfds.push_back(listener_id);
	listen(listener_id, 8);
	printf("Listener alive @ %s:%i\n", address, port);

	return 0;
}
// #Alpha5r1 :: selectah!!
// __select_active return values -1 = disconnect, 0 = nothing, 1 = write, 2 = read
int GenericNetworking::__select_active_state() {
	//#Alpha5
	int newsockfd;
	//#Alpha5r1
	int n = 0, rv = 0;
	fd_set set;
	struct timeval timeout;
	timeout.tv_sec	= 0;
	timeout.tv_usec	= 32;
	
	//printf("Listener looping through clientfds..\n\r");
	for(n = 0; n < clientfds.size(); n++) {
		active_id = clientfds[n];
		
		FD_ZERO(&set);
		FD_SET(active_id, &set);
			
		rv = select(clientfds[n] + 1, &set, NULL, NULL, &timeout);
		if(rv == -1) {
			printf("select(%i) returned -1, closing socket. I should propably reconnect ASAP..\n", active_id);
			close(active_id);
			return -1;
		} else if(rv == 0) {
			// __flush_write(io);
			return 1;
		} else if(rv > 0) {
			if(clientfds[n] == listener_id) {
				socklen_t clilen;
				struct sockaddr_in cli_addr;
				clilen = sizeof(cli_addr);
				newsockfd = accept(listener_id, (struct sockaddr *)&cli_addr, &clilen);
				if(newsockfd < 0) {
					printf("%i: failed to accept socket\n\r", __LINE__);
					continue;
				}
				printf("Adding newsockfd=%i to clientfds\n\r", newsockfd);
				clientfds.push_back(newsockfd);
				continue;
			}
			// __flush_read();
			return 2;
		}
	}
	return 0;
}
string GenericNetworking::__select_transfer_io(string io) {
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 32;

	fd_set set;
	FD_ZERO(&set);
	FD_SET(active_id, &set);

	// always flush
	__flush_write(io);
	
	int rv = select(active_id + 1, &set, NULL, NULL, &timeout);
	if(rv == -1) {
		printf("select(%i) returned -1, closing socket. I should propably reconnect ASAP..\n", active_id);
		close(active_id);
		// #Alpha5TODO :: reconnect
	} else if(rv == 0) {
		//__flush_write(io);
	} else if(rv > 0) {
		//printf("debug: select(%i) returned >1, reading ..\n", active_id);
		return __flush_read();
	}
	return "\n\r"; // ahh, but this is important.
}

// #Alpha5 :: THESE BELOW ARE READY
string GenericNetworking::__flush_read(void) {
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
	read(active_id, buf, len);
	commands = string(buf, len + 1);
	//printf("debug: __read() returned %s\n", buf);
	return commands;
}
int GenericNetworking::__flush_write(string buf) {
	int i, rv;
	Toolbox *box = new Toolbox();
	vector<string>	commands;
	commands = box->explode(buf, "\n");
	for(i = 0; i < commands.size(); i++)
		rv = __flush_write_sub(commands[i].c_str(), commands[i].size() + 1);
	return 0;
}
int GenericNetworking::__flush_write_sub(const char *text, int text_size) {
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