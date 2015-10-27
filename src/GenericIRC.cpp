#include "GenericIRC.hpp"
#include <stdio.h>

GenericIRC::GenericIRC(void) {
	printf("GenericIRC constructed\n");
}
int GenericIRC::feed(string line) {
	if(line.size() + 1 <= 3)
		return 0;

	//printf("GenericIRC::feed size=%i message=%s\n", line.size() + 1, line.c_str());
	return 0;
}
