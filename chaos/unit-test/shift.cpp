
#include <stdio.h>
#include <mutex>
// #include <functional>

int DIV_TABLE[1024][64];
int MOD_TABLE[1024][64];
int OFF_TABLE[16][64];
uint32_t MASK_TABLE[16][64];

void InitTable() {
    static struct Helper {
	Helper() {
printf( "lam1\n" );
        for( int i = 0; i < 1024; ++i )
        {
            for( int j = 1; j < 64; ++j )
            {
                DIV_TABLE[i][j] = i / j;
                MOD_TABLE[i][j] = i % j;
            }
        }
printf( "lam2\n" );
        for( int i = 0; i < 16; ++i )
        {
            uint32_t MASK0 = ( uint32_t( 1 ) << i ) - 1;
            for( int j = 0; j < 64; ++j )
            {
                OFF_TABLE[i][j] = i * j;
                MASK_TABLE[i][j] = ~( MASK0 << OFF_TABLE[i][j] );
            }
        }
printf( "lam3\n" );
		
	}
    } dummy;
}

int main() {
	InitTable();
	int x = 3;
	int y = x << 63;
	printf( "%d\n", y );
	return 0;
}
