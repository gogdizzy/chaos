// #include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "mem.h"



int main() {
	int N = 128 * 1024 * 1024;
	scanf( "%d", &N );

	printf( "init:   %zu %zu\n", getCurrentRSS(), getMallFree() );
for( int i = 0; i < 10; ++i ) {
	int *p = new int[N];
	memset( p, 0, sizeof(int) * N );
	printf( "new:    %zu %zu\n", getCurrentRSS(), getMallFree() );
	delete[] p;
	printf( "delete: %zu %zu\n", getCurrentRSS(), getMallFree() );
}
	
	return 0;
}
