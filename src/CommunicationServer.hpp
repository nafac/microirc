// CommunicationServer :: Text-based IPC hub.
#include "GenericNetworking.hpp"
class CommunicationServer {
	class ProcessDispatcher {
		//
	public:
		//
	};
	// process_dispatcher
	GenericNetworking *CommunicationHub;
	int		process_dispatcher(void);
	void		build_select_list(void);
	void		read_socks(void);
	void		handle_new_connection(void);
	void		deal_with_data(int listnum);
	void		setnonblocking(int sock);
	int		__process_dispatcher(void);
public:
	CommunicationServer(struct moduleconf *);
};
