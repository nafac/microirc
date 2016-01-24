MODULES_CPP = g++ -O0 -ggdb -Werror -std=gnu++11
MODULES_FINAL = mirccd.o Toolbox.o GenericNetworking.o mod_irc.o CommunicationServer.o CommunicationConnector.o

all: ${MODULES_FINAL} SharpConnect
	g++ ${MODULES_FINAL} -lpthread -o mirccd
SharpConnect:
	dmcs -out:SharpConnect src/SharpConnect.cs

mirccd.o: src/mirccd.hpp src/mirccd.cpp
	${MODULES_CPP} -c src/mirccd.cpp
Toolbox.o: src/Toolbox.hpp src/Toolbox.cpp
	${MODULES_CPP} -c src/Toolbox.cpp
GenericNetworking.o: src/GenericNetworking.hpp src/GenericNetworking.cpp
	${MODULES_CPP} -c src/GenericNetworking.cpp
CommunicationServer.o: src/CommunicationServer.hpp src/CommunicationServer.cpp
	${MODULES_CPP} -c src/CommunicationServer.cpp
CommunicationConnector.o: src/CommunicationConnector.hpp src/CommunicationConnector.cpp
	${MODULES_CPP} -c src/CommunicationConnector.cpp
mod_irc.o: src/modules/mod_irc.hpp src/modules/mod_irc.cpp
	${MODULES_CPP} -c src/modules/mod_irc.cpp

clean:
	rm -Rf ${MODULES_FINAL} SharpConnect mirccd

