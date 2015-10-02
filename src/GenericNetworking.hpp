using namespace std;
#include <string>
#include <vector>
// #alpha5 :: microkernel-based streaming chaos server 2015

enum position { LISTEN, CONNECT };

class GenericNetworking {
	// variables
	int				_fd;
	vector<string>	rbuf;
	vector<string>	wbuf;

//	position	__mode; // it is already determined

	// synchronous control
	void * 	_write		(void *);
	void * 	_read		(void *);
	// 
	int _connect(int port, char *address);
	int _listen(int port, char *address);
public:
	GenericNetworking(position __mode, int port, char *address);
	// 1. Chat!
	string __read();
	int __write();
	// 2. Text
	// 3. Binary
};
