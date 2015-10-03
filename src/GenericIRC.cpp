#include "GenericIRC.hpp"
#include <stdio.h>

GenericIRC::GenericIRC(void) {
	printf("GenericIRC constructed\n");
}
int GenericIRC::feed(string line) {
	printf("GenericIRC received '%s'\n", line.c_str());
	return 0;
}
