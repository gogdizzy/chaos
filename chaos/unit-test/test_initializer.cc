#include <cstdio>
#include <chrono>
#include <functional>
#include <initializer_list>


class benchmark {
    using fun = std::function< void() >;
public:
    benchmark( int catalog_width = 16, int item_width = 16 ) : catalog_width( catalog_width ), item_width( item_width ) {}

    void head( const char *catalog, std::initializer_list< const char * > heads ) {
        printf( "%*s", catalog_width, catalog );
        for( auto h : heads ) {
            printf( "%*s", item_width, h );
        }
        printf( "\n" );
    }

    void content( const char *catalog, std::initializer_list< const fun > funs ) {
        printf( "%*s", catalog_width, catalog );
        for( auto &f : funs ) {
            auto beg = std::chrono::high_resolution_clock::now();
            f();
            auto end = std::chrono::high_resolution_clock::now();
            printf( "%*lld", item_width, std::chrono::duration_cast< std::chrono::microseconds >(end - beg).count() );
        }
        printf( "\n" );
    }

private:
    int catalog_width;
    int item_width;
};

int f1( int a1, int a2, int a3, int a4, int a5 ) {
	return a1 + a2 + a3 + a4 + a5;
}

int f2( const std::initializer_list<int> &lst ) {
	int rv = 0;
	for( auto x : lst ) rv += x;
	return rv;
}

int test1( int N ) {
	int sum = 0;
	for( int i = 0; i < N; ++i ) {
		sum += f1( 1, 2, 3, 4, 5 );
	}
	return sum;
}

int test2( int N ) {
	int sum = 0;
	for( int i = 0; i < N; ++i ) {
		sum += f2( { 1, 2, 3, 4, 5 } );
	}
	return sum;
}
int main() {
	int N = 1000000;
printf( "%d %d\n", test1( N ), test2( N ) );
	benchmark bm( 20, 20 );
    bm.head( "", { "test1", "test2" } );
    bm.content( "time", { [=]() { test1( N ); }, [=]() { test2( N ); } } );
	
	return 0;
}
