using namespace std;
#include <string>
#include <vector>
// #alpha5 :: microkernel-based streaming chaos server 2015

enum position { LISTEN, CONNECT };

class GenericNetworking {
	// #Alpha5
	int _connect(int port, char *address);
	int _listen(int port, char *address);
	// 
	int			_fd;
	vector<int>	clientfds;
	// 
	string	__read();
//	int		__write();
	int		__flush_write(string);
	int		__flush_write_sub(const char *, int);
public:
	GenericNetworking(position __mode, int port, char *address);
	string	__select(string io);
	// #Alpha5r1
	int		__select_routing_module();
};
