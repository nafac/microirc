#include <stdio.h>
#include "mod_irc.hpp"
#include "../CommunicationConnector.hpp"

mod_irc::mod_irc()
{
	printf("mod_irc::mod_irc() called\n\r");
}
int mod_irc::connect(int hash)
{
	if(hash == 0) { /* do freaking retarded things like */ hash = rand() % 0xFFFFFF; }
	//
	CommunicationConnector *hub = new CommunicationConnector();
	//
	hub->__write(std::to_string(hash));
	hub->__read();
	return hash;
}