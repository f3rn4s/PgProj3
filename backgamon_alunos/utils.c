#include <stdlib.h>
#include <time.h>

int irand(int li, int ls) {
	return (rand() % (ls -li+1)) + li;
}


void rand_start() {
	srand(time(NULL));
}


int min(int a, int b) {
	return a < b ? a: b;
}

int max(int a, int b) {
	return a > b ? a: b;
}
