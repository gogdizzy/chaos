#include <cstdio>
#include <cstdarg>
#include <tuple>
#include <algorithm>
#include <chrono>
#include <functional>
#include <initializer_list>

#define ARGS_NUM(...)  (std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value)
#define ARGS_ARRAY(...) ARGS_NUM(__VA_ARGS__), __VA_ARGS__

enum class E : int8_t {
	A,
	B,
	C,
	D
};





int f0( E a, E b, E c, E d ) {
	return (int)a + (int)b + (int)c + (int)d;
}

int f1( int count, ... ) {
        va_list ap;
	va_start(ap, count);
	int sum = 0;
	for (int i = 0; i < count; i++) {
		sum += (int)va_arg(ap, E);
	}
	va_end(ap);
	return sum;
}

int f2( const std::initializer_list<E> &lst ) {
	int rv = 0;
	for( auto x : lst ) rv += (int)x;
	return rv;
}

template <typename... Types>
int f3(Types&& ...types) {
	int sum = 0;
	E x[] = { types... };

	for( int i = 0; i < sizeof...(types); ++i ) {
		sum += (int)x[i];
	}
	return sum;
}

template <typename... Types>
int f4(Types&& ...types) {
	int sum = 0;
	E x[] = { types... };

	for( auto z : x ) {
		sum += (int)z;
	}
	return sum;
}

int test0( int N ) {
	int sum = 0;
	for( int i = 0; i < N; ++i ) {
		sum += f0( E::A, E::B, E::C, E::D );
	}
	return sum;
}

int test1( int N ) {
	int sum = 0;
	for( int i = 0; i < N; ++i ) {
		sum += f1( ARGS_ARRAY( E::A, E::B, E::C, E::D ) );
	}
	return sum;
}

int test2( int N ) {
	int sum = 0;
	for( int i = 0; i < N; ++i ) {
		sum += f2( { E::A, E::B, E::C, E::D } );
	}
	return sum;
}
int test3( int N ) {
	int sum = 0;
	for( int i = 0; i < N; ++i ) {
		sum += f3( E::A, E::B, E::C, E::D );
	}
	return sum;
}
int test4( int N ) {
	int sum = 0;
	for( int i = 0; i < N; ++i ) {
		sum += f4( E::A, E::B, E::C, E::D );
	}
	return sum;
}

int main() {
	int N = 10000000;
int dummy = 0;
	benchmark bm( 8, 24 );
    bm.head( "", { "DirectCall", "VariadicParams", "InitializerList", "VariadicTmplSizeof", "VariadicTmplRangeFor" } );
    bm.content( "time", { [&]() { dummy += test0( N ); }, [&]() { dummy += test1( N ); }, [&]() { dummy += test2( N ); }, [&]() { dummy += test3( N ); }, [&]() { dummy += test4( N ); } } );
printf( "dummy = %d\n", dummy );
	
	return 0;
}
