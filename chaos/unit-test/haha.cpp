#include <stdio.h>

int main() {
	int *p = new int[1000000000];
	p[0] = 1;
	p[1000] = 2;
	p[1000000] = 3;
	p[1000000000-1] = 4;
	getchar();
	return 0;
}
