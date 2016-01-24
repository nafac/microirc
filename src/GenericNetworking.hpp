#ifndef _GENERICNETWORKING_H_
#define _GENERICNETWORKING_H_
using namespace std;
#include <string>
#include <vector>
//#Alpha5 :: A microkernel-based streaming chaos server 2016.

//#Alpha5r4
class UniversalNetwork {
	public:
		UniversalNetwork();
		// universal functionality
		int IPV4CreateSocket(char *address, char *port);
		int IPV6CreateSocket(char *address, char *port);
		// client functionality
		int IPV4Connect(char *address, char *port);
		int IPV6Connect(char *address, char *port);
		// server functionality
		int IPV6Server(char *address, char *port);
		// 
		string	__read(int sfd, int *rv);
		int		__write(int sfd, string text);
		// do NOT attempt to use for selected !!
		int		__select_socket_state(int sockfd);
		//
		int		get_fd();
	private:
		// vars
		int main_fd;
		struct addrinfo *resolver;
		// methods
		int			__write_sub(int sfd, const char *, int);
	protected:
		//	
};
//#Alpha5r3
class GenericNetworking {
	//#Alpha5r1
	int			listener_id;
	vector<int>	clientfds;
//	fd_set		fds;			// unused?
	//#Alpha5r1
//int		__flush_read(int fd, string *rbuf);										// converted
//int		__flush_write(int active_fd, string io);							// converted
//int		__flush_write_sub(int active_fd, const char *, int);	// converted
	int		__accept();																						// -
	//#Alpha5r3
	//int		static_connector_router;
public:
	GenericNetworking(); // Just call it.
	//#Alpha5
	int		IPV4Connect(char *address, char *port);
	int		IPV6Connect(char *address, char *port);
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
	
	//#Alpha5r4
	UniversalNetwork *CommonNetwork;
};

#endif
