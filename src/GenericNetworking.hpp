using namespace std;
#include <string>
#include <vector>
// #alpha5 :: microkernel-based streaming chaos server 2015

enum position { LISTEN, CONNECT };

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
	GenericNetworking(/*position __mode, int port, char *address*/); // Just call it.
	//#Alpha5
	int		_connect(int port, char *address);
	int		_listen(int port, char *address);
	//#Alpha5r1
	int		__socket_state(int target_fd);
	int		__server_select(int *active_fd);
	string	__connector_transfer_io(int static_connector_router, string io);
	// ..its a freaking breed bot god damnit.
	//#Alpha5r3
	int		__select_routing_module();
};
