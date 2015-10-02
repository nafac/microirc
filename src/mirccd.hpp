// built-in modules
enum	modules {
	COMMUNICATION_HUB,					// The Communication Hub
	IRC_LIB, IRC_CLIENT, IRC_SERVER 	// Communication Connectors
};
// first pass configuration
struct moduleconf {
	modules name;
	char *address;
	int port;
};
