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
	int			active_id;
	vector<int>	clientfds;
//	fd_set		fds;			// unused?
	//#Alpha5r1
	string	__read(void);
	int		__flush_write(string);
	int		__flush_write_sub(const char *, int);
public:
	GenericNetworking(position __mode, int port, char *address);
	//#Alpha5r1
	int		__select_active(void);
	string	__select(string io);
	// ..its a freaking breed bot god damnit.
	//#Alpha5r3
	int		__select_routing_module();
};
