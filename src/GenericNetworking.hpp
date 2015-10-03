using namespace std;
#include <string>
#include <vector>
// #alpha5 :: microkernel-based streaming chaos server 2015

enum position { LISTEN, CONNECT };

class GenericNetworking {
/*	// Alpha5 :: obsolete?
	vector<string>	rbuf;
	vector<string>	wbuf;
*/

	// #Alpha5
	int _connect(int port, char *address);
	int _listen(int port, char *address);
	// 
	int _fd;
	// 
	string	__read();
//	int		__write();
	int		__flush_write(string *);
public:
	GenericNetworking(position __mode, int port, char *address);
	string	__select(string io);
	string	flush_read();
	int		flush_write();
};
