// CommunicationServer :: Text-based IPC hub.
#include <vector>
#include <string>
#include "GenericNetworking.hpp"

class CommunicationConnector {
	UniversalNetwork *router_one, *router_two;
public:
	//#Alpha6
	CommunicationConnector();
	// 
	int AddEndpoint(char *address, char *port);
	// 
	int __static_route_io();
	int __read();
	int __write(string text);
};
