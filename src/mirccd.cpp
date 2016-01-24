#include "CommunicationServer.hpp"
#include "CommunicationConnector.hpp"
//#include "GenericNetworking.hpp"
#include "mirccd.hpp"
#include <pthread.h>
#include <unistd.h>
#include <strings.h>

/* #Alpha6 */
// it is still disgusting

// 
int main(int, char **);
// 
//#Alpha6 :: Implement core and backend functionality exception handling and keep-alive !!
void *CommunicationServer_dp(void *)
{
	CommunicationServer();													// create endpoint 1 (IPCHub)
	return NULL;															// keepalive
}
void *IRC_dp(void *)
{
	// 
	CommunicationConnector	*IPCH = new CommunicationConnector();			// create endpoint 1
	IPCH->AddEndpoint((char *)"irc.se.quakenet.org", (char *)"6667");		// create endpoint 2
	IPCH->__static_route_io();												// run bridge
	return NULL;															// keepalive
}
// 
int main(int argc, char **argv)
{
	printf("mirccd 'Chaos Server 2016 #Alpha6' started.\n");
	// 
	pthread_t CommunicationServer_t;
	pthread_create(&CommunicationServer_t, NULL, &CommunicationServer_dp, NULL);
	//
	sleep(1);
	// 
	pthread_t IRC_t;
	pthread_create(&IRC_t, NULL, &IRC_dp, NULL);
	//
	while (1) { sleep(60); }
	return -1;
}
/*
void *thread_dispatcher(void *arg) {
	// built-in modules and first pass configuration.
	struct moduleconf *args = (struct moduleconf *)arg;
	if (args->name == COMMUNICATION_HUB) {
		CommunicationServer((struct moduleconf *)arg);
	}
	else if (args->name == IRC_LIB || args->name == IRC_CLIENT || args->name == IRC_SERVER) {
		CommunicationConnector((struct moduleconf *)arg);
		// embed these as modules !!
	}
	return NULL;
}
*/
/*
int main_old(int argc, char **argv) {
	
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
	args.name		= IRC_CLIENT;
	args.address	= (char *)"chat.freenode.net";
	args.port		= 6667;
	pthread_create(&(thread_id[3]), NULL, &thread_dispatcher, (void *)&args);
	sleep(1);
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

	printf("main finalized, this should not happen!! do something useful..\n\r");
}
*/