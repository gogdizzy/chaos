//
// Created by 关鑫 on 2019-05-30.
//

#include <algorithm>
#include <cstdio>
#include <queue>
#include <random>
#include <map>
#include <unordered_map>

#include "utils/AllocatorTracker.h"
#include "utils/Benchmark.h"

#define MAXV 1000000

#define TESTN 200

std::default_random_engine randEngine;
std::uniform_int_distribution< int > intRand( 0, MAXV - 1 );
std::uniform_real_distribution< float > floatRand( 0.f, 100.f );


typedef std::unordered_map< int32_t, int64_t, std::hash< int32_t >, std::equal_to< int32_t >, AllocatorTracker< std::pair< const int32_t, int64_t > > > Hash32_64;
typedef std::unordered_map< int32_t, Hash32_64, std::hash< int32_t >, std::equal_to< int32_t >, AllocatorTracker< std::pair< const int32_t, Hash32_64 > > > Hash32_32_64;
typedef std::unordered_map< int64_t, int64_t, std::hash< int64_t >, std::equal_to< int64_t >, AllocatorTracker< std::pair< const int64_t, int64_t > > > Hash64_64;

int32_t compact( int32_t x, int32_t y ) {
    return ( x << 16 ) | ( y & 0xFFFF );
}

int64_t compact( int64_t x, int64_t y, int64_t x2, int64_t y2 ) {
    return ( (x & 0xFFFF) << 48 ) | ( ( y & 0xFFFF ) << 32) | ( ( x2 & 0xFFFF ) << 16) | ( y2 & 0xFFFF );
}

void testTwoLayer() {
    Hash32_32_64 hashMap;

    for( int uy = 0; uy < TESTN; ++uy )
    for( int ux = 0; ux < TESTN; ++ux ) {
        for( int vy = uy - 1; vy <= uy + 1; ++vy )
        for( int vx = ux - 1; vx <= ux + 1; ++vx ) {
            if( vx != ux || vy != uy ) {
                hashMap[compact(ux, uy)][compact(vx, vy)] = intRand( randEngine );
            }
        }
    }

//    for( int i = 0; i < TESTN / 4; ++i ) {
//        auto u = intRand(randEngine);
//        for( int k = 0; k < 4; ++k ) hashMap[u][intRand(randEngine)] = intRand(randEngine);
//    }

    printf( "==================== Two Layer Begin ====================\n" );

    printf( "size: %zu\n", hashMap.size() );

    int tot = hashMap.get_allocator().get_cur_size();
    for( auto& it : hashMap ) {
        tot += it.second.get_allocator().get_cur_size();
    }

    printf( "memory: %d\n", tot );

    printf( "==================== Two Layer End ====================\n" );
}

void testOneLayer() {
    Hash64_64 hashMap;
    for( int uy = 0; uy < TESTN; ++uy )
        for( int ux = 0; ux < TESTN; ++ux ) {
            for( int vy = uy - 1; vy <= uy + 1; ++vy )
                for( int vx = ux - 1; vx <= ux + 1; ++vx ) {
                    if( vx != ux || vy != uy ) {
                        hashMap[compact( ux, uy, vx, vy )] = intRand( randEngine );
                    }
                }
        }

    printf( "==================== One Layer Begin ====================\n" );

    printf( "size: %zu\n", hashMap.size() );

    int tot = hashMap.get_allocator().get_cur_size();

    printf( "memory: %d\n", tot );

    printf( "==================== One Layer End ====================\n" );
}

typedef std::map< int64_t, int64_t, std::less< int64_t >, AllocatorTracker< std::pair< const int64_t, int64_t > > > TreeMap64_64;

void testOneLayer2() {
    TreeMap64_64 treeMap;
    for( int i = 0; i < TESTN; ++i ) {
        treeMap[intRand(randEngine)] = intRand(randEngine);
    }

    printf( "==================== One Layer 2 Begin ====================\n" );

    printf( "size: %zu\n", treeMap.size() );

    int tot = treeMap.get_allocator().get_cur_size();

    printf( "memory: %d\n", tot );

    printf( "==================== One Layer 2 End ====================\n" );
}

void init() {
    printf( "sizeof size_t: %d\n", sizeof( size_t ) );
    Hash32_64 hashMap;
    printf( "init 0: %lld\n", hashMap.get_allocator().get_cur_size() );
    for( int i = 0; i < 64; ++i ) {
        hashMap[intRand(randEngine)] = intRand( randEngine );
        printf( "init %d: %lld\n", i + 1, hashMap.get_allocator().get_cur_size() );
    }
}

int main() {

    init();

    Benchmark bm;

    auto oneLayerTime = bm.run_once( [](){ testOneLayer(); } );
    auto twoLayerTime = bm.run_once( [](){ testTwoLayer(); } );

    printf( "%lld\n", oneLayerTime );
    printf( "%lld\n", twoLayerTime );

    return 0;
}
