using namespace std;
#include <string>
#include <vector>
// #alpha5 :: microkernel-based streaming chaos server 2015

enum position { LISTEN, CONNECT };

class GenericNetworking {
	//#Alpha5
	int _connect(int port, char *address);
	int _listen(int port, char *address);
	//#Alpha5r1
	int			connector_id;
	int			listener_id;
	int			active_id;	// remove next round, unicurses curse is bad
	vector<int>	clientfds;
//	fd_set		fds;			// unused?
	//#Alpha5r1
	string	__flush_read(void);
	int		__flush_write(string);
	int		__flush_write_sub(const char *, int);
public:
	GenericNetworking(position __mode, int port, char *address);
	//#Alpha5r1
	int		__select_active_state(void);
	string	__select_transfer_io(string io);
	// ..its a freaking breed bot god damnit.
	//#Alpha5r3
	int		__select_routing_module();
};
