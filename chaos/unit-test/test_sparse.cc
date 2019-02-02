#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mem.h"
#include "SparseGridMap.h"


void test( int xs, int ys ) {
	SparseGridMap<SparseGridMap<int64_t> > oldCosts;
	for( int xu = xs; xu < xs + 100; ++xu ) {
		for( int yu = ys; yu < ys + 100; ++yu ) {
			for( int xv = xu - 1; xv <= xu + 1; ++xv ) {
				for( int yv = yu - 1; yv <= yu + 1; ++yv ) {
					if( xu != xv || yu != yv ) {
						 oldCosts.At( xs + rand() % 1024, ys + rand() % 1024 ).At( xs + rand()%1024, ys + rand() % 1024 ) = 1;
						// oldCosts.At( xu, yu ).At( xv, yv ) = 1;
					}
				}
			}
		}

	}

int tot = 0;
oldCosts.ForEach([&](const int32_t& ux, const int32_t& uy, const SparseGridMap<int64_t>& tmp)
        {
            tmp.ForEach([&](const int32_t& vx, const int32_t& vy, const int64_t& oldC)
                {  tot += oldC;
	        } );
	} );
}

int main() {
// mallopt( M_MMAP_THRESHOLD, 1 );
	srand( time(NULL) );
	printf( "curRSS:%zu\n", getCurrentRSS() );
	for( int i = 0; i < 100; ++i ) {
		// test( rand(), rand() );
		test( 0, 0 );
		printf( "curRSS:%zu\n", getCurrentRSS() );
	}
	return 0;
}

