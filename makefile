MODULES_CPP = g++ -O0 -ggdb -Werror -std=gnu++11
MODULES_FINAL = mirccd.o Toolbox.o GenericIRC.o GenericNetworking.o CommunicationServer.o CommunicationConnector.o

all: ${MODULES_FINAL}
	g++ ${MODULES_FINAL} -lpthread -o mirccd

mirccd.o: src/mirccd.hpp src/mirccd.cpp
	${MODULES_CPP} -c src/mirccd.cpp
Toolbox.o: src/Toolbox.hpp src/Toolbox.cpp
	${MODULES_CPP} -c src/Toolbox.cpp
GenericIRC.o: src/GenericIRC.hpp src/GenericIRC.cpp
	${MODULES_CPP} -c src/GenericIRC.cpp
GenericNetworking.o: src/GenericNetworking.hpp src/GenericNetworking.cpp
	${MODULES_CPP} -c src/GenericNetworking.cpp
CommunicationServer.o: src/CommunicationServer.hpp src/CommunicationServer.cpp
	${MODULES_CPP} -c src/CommunicationServer.cpp
CommunicationConnector.o: src/CommunicationConnector.hpp src/CommunicationConnector.cpp
	${MODULES_CPP} -c src/CommunicationConnector.cpp

clean:
	rm -Rf ${MODULES_FINAL}

