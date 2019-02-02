#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void GenCore() {
  printf( "GenCore 1\n" );
  *(char*)nullptr = 0;
  printf( "GenCore 2\n" );
}

void Handler( int sig ) {
  printf( "Handler 1\n" );
}

int main() {
	signal( SIGSEGV, Handler );
	GenCore();
	return 0;
}
