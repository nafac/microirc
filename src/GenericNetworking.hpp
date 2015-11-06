#ifndef _GENERICNETWORKING_H_
#define _GENERICNETWORKING_H_
using namespace std;
#include <string>
#include <vector>
// #alpha5 :: microkernel-based streaming chaos server 2015
//enum position { LISTEN, CONNECT };

class GenericNetworking {
	//#Alpha5r1
	int			listener_id;
	vector<int>	clientfds;
//	fd_set		fds;			// unused?
	//#Alpha5r1
	string		__flush_read(int active_fd);
	int			__flush_write(int active_fd, string io);
	int			__flush_write_sub(int active_fd, const char *, int);
	int			__accept();
	//#Alpha5r3
	//int			static_connector_router;
public:
	GenericNetworking(); // Just call it.
	//#Alpha5
	int		_connect(int port, char *address);
	//int		_listen(int port, char *address);

	//#Alpha5r1
	// Generic socket area
	int		__socket_state(int target_fd);
	// CommunicationServer area
	int		__server_select(int *active_fd);
	// CommunicationConnector area
	int		__connector_static_router(int source, int destination);

	//#Alpha5r2
	int		static_communication_router_fd; // It's exposed all right? yo!
	// ..its a freaking breed bot god damnit.

	//#Alpha5r3
	int		__select_routing_module();
};
#endif
