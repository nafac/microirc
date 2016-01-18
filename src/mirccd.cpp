#include "CommunicationServer.hpp"
#include "CommunicationConnector.hpp"
//#include "GenericNetworking.hpp"
#include "mirccd.hpp"
#include <pthread.h>
#include <unistd.h>
#include <strings.h>

int main(int, char **);

void *thread_dispatcher(void *arg) {
	// built-in modules and first pass configuration.
	struct moduleconf *args = (struct moduleconf *)arg;
	if(args->name == COMMUNICATION_HUB) {
		CommunicationServer((struct moduleconf *)arg);
	} else if(args->name == IRC_LIB || args->name == IRC_CLIENT || args->name == IRC_SERVER) {
		CommunicationConnector((struct moduleconf *)arg);
		// embed these as modules !!
	}
	return NULL;
}

int main(int argc, char **argv) {
	printf("mirccd 'Chaos Server 2016 #Alpha5r4' started.\n");
	// ultra threaded dispatch server
	pthread_t thread_id[4096];
	// expandable built-in modules configuration. see above.
	moduleconf args;
	// #Alpha5TODO :: attempt to vectorize thread container.
	// 
	// The Communication Hub
	args.name		= COMMUNICATION_HUB;
	pthread_create(&(thread_id[0]), NULL, &thread_dispatcher, (void *)&args);
	sleep(1);
	// Communication Connectors
	/// IRC_LIB
	args.name	= IRC_LIB;
	pthread_create(&(thread_id[1]), NULL, &thread_dispatcher, (void *)&args);
	sleep(1);
	/// IRC_CLIENT
	//// Quakenet
	args.name	= IRC_CLIENT;
	args.address	= (char *)"irc.se.quakenet.org";
	args.port			= (char *)"6667";
	pthread_create(&(thread_id[2]), NULL, &thread_dispatcher, (void *)&args);	
	sleep(1);
	//// Freenode TODO: The supersecret handshake is not working, yet. I got more important things to do meanwhile.
/*
	args.name		= IRC_CLIENT;
	args.address	= (char *)"chat.freenode.net";
	args.port		= 6667;
	pthread_create(&(thread_id[3]), NULL, &thread_dispatcher, (void *)&args);
	sleep(1);
*/
	args.name	= IRC_SERVER;
	args.address	= (char *)"0.0.0.0";
	args.port			= (char *)"6668";
	pthread_create(&(thread_id[4]), NULL, &thread_dispatcher, (void *)&args);

										
	// debugging..
	//GenericNetworking *nio = new GenericNetworking(CONNECT, 6669, (char *)"127.0.0.1");

	while(1) { sleep(60); } // this is ridiculous

	// just do it, just forget it
	//GenericNetworking *client = new GenericNetworking();
	//client->client(6667, "irc.quakenet.org");
	//GenericNetworking *server = new GenericNetworking();
	//server->server(6669, "localhost");

	// CONTAIN
	// UI
	// SERVERS
	// USERS
	// ..this does not work..!!

	printf("main finalized.. do something useful..\n");
}
