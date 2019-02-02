#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>

#include "mem.h"

#include <chrono>
namespace st = std::chrono;

void getrss() {
	int who = RUSAGE_SELF; 
	struct rusage usage; 
	int ret = getrusage(who, &usage);
	printf( "getrusage | maxrss = %ld idrss = %ld ixrss = %ld isrss = %ld\n", usage.ru_maxrss, usage.ru_idrss, usage.ru_ixrss, usage.ru_isrss );

	struct sysinfo info;
	sysinfo( &info );
	printf( "sysinfo | totalram = %ld freeram = %ld sharedram = %ld bufferram = %ld totalswap = %ld freeswap = %ld mem_unit = %u\n",
		info.totalram, info.freeram, info.sharedram, info.bufferram,
		info.totalswap, info.freeswap, info.mem_unit );

	long free = 0, buffer = 0, cached = 0;
	
	FILE* fp = NULL;
	if ( (fp = fopen( "/proc/meminfo", "r" )) != NULL ) {
		if( fscanf( fp, "%*s%*s%*s" "%*s%ld%*s" "%*s%ld%*s" "%*s%ld%*s", &free, &buffer, &cached ) != 3 ) {
			printf( "fscanf error\n" );
		}
		fclose( fp );	
	}
	printf( "meminfo | free = %ld buffer = %ld cached = %ld avail = %u\n", free * 1024, buffer * 1024, cached * 1024, getAvailMem() );

	printf( "mem.h | getCurrentRSS = %u getPeakRSS = %u\n", getCurrentRSS(), getPeakRSS() );
}

int main() {
	const int N = 10 * 1024 * 1024;
	getrss();
	int *data = new int[N];
	getrss();
	data[0] = 1;
	getrss();
	data[1023] = 1;
	getrss();
	for( int i = 0; i < N; ++i) data[i] = i;
	getrss();
	delete data;
	getrss();

{
auto beg = st::high_resolution_clock::now();
	for( int i = 0; i < 10000; ++i ) auto x = getCurrentRSS();
auto end = st::high_resolution_clock::now();
printf( "10000 times getCurrentRSS : %20lld\n", st::duration_cast< st::microseconds >(end - beg).count() );
}

{
struct sysinfo info;
auto beg = st::high_resolution_clock::now();
	for( int i = 0; i < 10000; ++i ) auto x = sysinfo( &info );
auto end = st::high_resolution_clock::now();
printf( "10000 times sysinfo : %20lld\n", st::duration_cast< st::microseconds >(end - beg).count() );
}
{
auto beg = st::high_resolution_clock::now();
	for( int i = 0; i < 10000; ++i ) auto x = getAvailMem();
auto end = st::high_resolution_clock::now();
printf( "10000 times meminfo : %20lld\n", st::duration_cast< st::microseconds >(end - beg).count() );
}


{
auto beg = st::high_resolution_clock::now();
	for( int i = 0; i < 10000; ++i ) printf( "curRSS:%zu availMem:%zu mallFree:%zu\n", getCurrentRSS(), getAvailMem(), getMallFree() );
auto end = st::high_resolution_clock::now();
printf( "10000 times mix : %20lld\n", st::duration_cast< st::microseconds >(end - beg).count() );
}

	return 0;
}
