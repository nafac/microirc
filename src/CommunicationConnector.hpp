// CommunicationServer :: Text-based IPC hub.
#include <vector>
#include <string>
#include "GenericNetworking.hpp"

class CommunicationConnector {
	//#Alpha5	:: irssi <=> mirccd <=> quakenet proto
	//#Alpha5r1	:: Static Connector Router
	GenericNetworking *router_one, *router_two;
public:
	CommunicationConnector(struct moduleconf *);
	int static_route_io(void);
};
