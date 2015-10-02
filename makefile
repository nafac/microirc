MODULES_FINAL=mirccd.o GenericNetworking.o CommunicationServer.o CommunicationConnector.o

all: ${MODULES_FINAL}
	g++ ${MODULES_FINAL} -lpthread -o mirccd

mirccd.o: src/mirccd.cpp
	g++ -O0 -ggdb -Werror -std=gnu++11 -c src/mirccd.cpp

CommunicationServer.o: src/CommunicationServer.hpp src/CommunicationServer.cpp
	g++ -O0 -ggdb -Werror -std=gnu++11 -c src/CommunicationServer.cpp
CommunicationConnector.o: src/CommunicationConnector.hpp src/CommunicationConnector.cpp
	g++ -O0 -ggdb -Werror -std=gnu++11 -c src/CommunicationConnector.cpp

GenericNetworking.o: src/GenericNetworking.hpp src/GenericNetworking.cpp
	g++ -O0 -ggdb -Werror -std=gnu++11 -c src/GenericNetworking.cpp

clean:
	rm -Rf *.o
