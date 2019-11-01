//
// Created by 关鑫 on 2019-05-30.
//

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <queue>
#include <random>
#include <unordered_map>

#include <boost/heap/fibonacci_heap.hpp>
#include <boost/heap/binomial_heap.hpp>
#include <boost/heap/d_ary_heap.hpp>

#include "utils/AllocatorTracker.h"
#include "utils/Benchmark.h"

#include "PriorityQueueX.h"
#include "PriorityQueueU.h"

#define MAXV 1000000

#define TESTN 40000

std::default_random_engine randEngine;
std::uniform_int_distribution< int > intRand( 0, MAXV - 1 );
std::uniform_real_distribution< float > floatRand( 0.f, 100.f );
std::uniform_real_distribution< float > smallFloatRand( 0.f, 10.f );

int32_t compact( int32_t x, int32_t y ) {
    return ( x << 16 ) | ( y & 0xFFFF );
}

int64_t compact( int64_t x, int64_t y, int64_t x2, int64_t y2 ) {
    return ( (x & 0xFFFF) << 48 ) | ( ( y & 0xFFFF ) << 32) | ( ( x2 & 0xFFFF ) << 16) | ( y2 & 0xFFFF );
}

struct Vertex {
    int x;
    int y;
    int data;
    float dist;

    Vertex() {
        x = intRand( randEngine );
        y = intRand( randEngine );
        dist = floatRand( randEngine );
    }
};

Vertex vertexes[MAXV];

// dup算法，需要额外存储dist，记录入队时候的值
// 如果出队时和节点自身的值不同，则说明是过期数据
// 缺点：无法获取队列size，因为里面有过期值，内存稍大
// 优点：实现简单，用std::priority_queue即可

struct DupKey {
    Vertex *vertex;
    float dist;

    DupKey() {}

    DupKey( Vertex *v, float d ) : vertex( v ), dist( d ) {}
};

struct DupKeyCmp {
    bool operator()( const DupKey &a, const DupKey &b ) const { return a.dist > b.dist; }
};

typedef AllocatorTracker< DupKey > DupTracker;

typedef std::priority_queue< DupKey, std::vector< DupKey, DupTracker >, DupKeyCmp > DupHeap;


void testDup() {

    DupTracker tracker;

    DupHeap distQ( tracker );

    for( int i = 0; i < 8 * TESTN; ++i ) distQ.emplace( vertexes + intRand( randEngine ), floatRand( randEngine ) );

    printf( " =============== DupHeap Begin =============\n" );

    printf( "HeapSize: %zu\n", distQ.size() );
    printf( "MemorySize: %lld %lld\n", tracker.get_cur_size(), tracker.get_peak_size() );

    printf( " =============== DupHeap End =============\n" );

//    while( distQ.size() > 0 ) {
//        auto point = distQ.top();
//        distQ.pop();
//        auto dist = point.vertex->dist;
//        if( dist != point.dist ) continue;
//        // std::__heap
//        // TODO 处理相邻的数据点
//        for( auto x = point.x - 1; x <= point.x + 1; x++ ) {
//            for( auto y = point.y - 1; y <= point.y + 1; y++ ) {
//                if( ( x != point.x || y != point.y ) && point.dist < BUBBLE_SIZE && rect.IsInRect( x, y ) ) {
//                    auto &cell = spLocalBubbledMap->ToUnsafe().At( x, y );
//                    if( IsNullRef( cell ) ) continue;
//                    float newDist = dist + GetDistance( point.x, point.y, x, y );
//                    if( newDist < cell ) {
//                        cell = newDist;
//                        distQ.push( DupKey( newDist, x, y ) );
//                    }
//                }
//            }
//        }
//    }
}

// 使用fibonacci堆，它会给每个插入的点返回一个handle，用于内部定位
// 那么外部就要记录节点和handle的映射关系，例如使用unordered_map
// 缺点：内存较大，运行时间的常数项较大
// 优点：使用较简单

struct FibKey {
    int32_t point;
    Vertex *vertex;
    int64_t k1;
    int64_t k2;

    FibKey() : vertex( nullptr ) {}
    FibKey( Vertex *v ) : vertex( v ) {}
};

struct FibKeyCmp {
    bool operator()( const FibKey &a, const FibKey &b ) const { return a.vertex->dist > b.vertex->dist; }
};

typedef AllocatorTracker< FibKey > FibTracker;

typedef boost::heap::fibonacci_heap< FibKey, FibTracker, boost::heap::compare< FibKeyCmp > > FibHeap;

void testFibonacci() {

    FibHeap distQ;
    std::unordered_map< int, FibHeap::handle_type, std::hash< int >, std::equal_to< int >,
            AllocatorTracker< std::pair< const int, FibHeap::handle_type > > > handleMap;

    for( int i = 0; i < TESTN; ++i ) {
        int idx = intRand( randEngine );
        auto& v = vertexes[idx];
        handleMap.emplace( compact( v.x, v.y ), distQ.emplace( vertexes + idx ) );
    }

    for( int i = 0; i < TESTN; ++i ) {
        auto key = distQ.top();
        auto& v = *(key.vertex);
        auto handle = handleMap[compact( v.x, v.y )];
        v.dist -= smallFloatRand( randEngine );
        distQ.update( handle, key );
    }

    while( distQ.size() > 0 ) {
        // distQ.top();
        distQ.pop();
    }


    printf( " =============== FibHeap Begin =============\n" );

    printf( "HeapSize: %zu\n", distQ.size() );
    printf( "HeapMemory: %lld %lld\n", distQ.get_allocator().get_cur_size(), distQ.get_allocator().get_peak_size() );
    printf( "MapMemory: %lld %lld\n", handleMap.get_allocator().get_cur_size(),
            handleMap.get_allocator().get_peak_size() );

    printf( " =============== FibHeap End =============\n" );
}

std::unordered_map< int32_t, int32_t, std::hash< int32_t>, std::equal_to< int32_t >,
        AllocatorTracker< std::pair< const int32_t, int32_t > > > pqxHandleMap;

struct PqxKey {
    int loc;
    // int loc2;
    Vertex *vertex;
    int64_t k1;
    int64_t k2;

    PqxKey() : loc( 0 ), vertex( nullptr ) {}
    PqxKey( Vertex *v ) : vertex( v ) {}
    PqxKey( const PqxKey &other ) : loc( other.loc ), vertex( other.vertex ) {}
    PqxKey( PqxKey &&other ) : loc( other.loc ), vertex( other.vertex ) {
        other.vertex = nullptr;
    }

    PqxKey &operator=( PqxKey &&other ) {

        vertex = other.vertex;
        other.vertex = nullptr;
        pqxHandleMap[compact(vertex->x, vertex->y)] = this->loc;
        // printf( "write %p = %d\n", vertex, this->loc );
        return *this;
    }
};

struct PqxKeyLoc {
    int& operator()( const PqxKey &a ) const { return pqxHandleMap[compact( a.vertex->x, a.vertex->y )]; }
};

struct PqxKeyCmp {
    bool operator()( const PqxKey &a, const PqxKey &b ) const { return a.vertex->dist > b.vertex->dist; }
};

typedef AllocatorTracker< PqxKey > PqxTracker;

typedef robot::PriorityQueueX< PqxKey, PqxKeyLoc, std::deque< PqxKey, PqxTracker >, PqxKeyCmp > PqxHeap;

void testPriorityQueueX() {

    PqxTracker tracker;

    PqxHeap distQ( tracker );

    std::unordered_map< int, int, std::hash< int >, std::equal_to< int >,
            AllocatorTracker< std::pair< const int, int > > > handleMap;

    int last = -1;

    for( int i = 0; i < 10; ++i ) {
        last = intRand( randEngine );
        distQ.emplace( vertexes + last );
    }

//    printf( "erase %f\n", vertexes[last].dist );
//    distQ.erase( PqxKey( vertexes + last ) );

    float delta = 100.f;
    printf( "update %f -> %f\n", vertexes[last].dist, vertexes[last].dist + delta );
    vertexes[last].dist += delta;
    distQ.update( vertexes + last );

    while( distQ.size() > 0 ) {
        printf( "read %p = %f\n", distQ.top().vertex, distQ.top().vertex->dist );
        distQ.pop();
    }

    for( int i = 0; i < TESTN; ++i ) {
        distQ.emplace( vertexes + intRand( randEngine ) );
    }

    for( int i = 0; i < TESTN; ++i ) {
        auto key = distQ.top();
        auto& v = *(key.vertex);
        v.dist -= smallFloatRand( randEngine );
        distQ.update( &v );
    }

    while( distQ.size() > 0 ) {
//        distQ.top();
        distQ.pop();
    }

    printf( " =============== PqxHeap Begin =============\n" );

    printf( "HeapSize: %zu\n", distQ.size() );
    printf( "HeapMemory: %lld %lld\n", tracker.get_cur_size(), tracker.get_peak_size() );
    printf( "MapMemory: %lld %lld\n", pqxHandleMap.get_allocator().get_cur_size(), pqxHandleMap.get_allocator().get_peak_size() );

    printf( " =============== PqxHeap End =============\n" );
}


typedef std::unordered_map< int32_t, int32_t, std::hash< int32_t >, std::equal_to< int32_t >,
        AllocatorTracker< std::pair< const int32_t, int32_t > > > PquMap;

struct PquKey : public robot::PquKeyBase {
    int32_t point;
    Vertex *vertex;
    int64_t k1;
    int64_t k2;

    PquKey() : vertex( nullptr ) {}
    PquKey( Vertex *v ) : point( compact(v->x, v->y) ), vertex( v ) {}
    PquKey( const PquKey &other ) : point( other.point ), vertex( other.vertex ) {}
    PquKey( PquKey &&other ) : point( std::move( other.point) ), vertex( std::move( other.vertex ) ) {
        other.vertex = nullptr;
    }

//    int32_t getLoc() {
//        if( !locator || !vertex ) return -1;
//        return (*(PquMap*)locator)[compact(vertex->x, vertex->y)];
//    }

    int32_t getLoc() {
        if( !locator || !vertex ) return -1;
        auto &locMap = *(PquMap*)locator;
        auto it = locMap.find(point);

        return it == locMap.end() ? -1 : it->second;
    }

    void setLoc( int32_t loc ) {
        this->loc = loc;
        if( locator ) (*(PquMap*)locator)[compact(vertex->x, vertex->y)] = this->loc;
    }

    void eraseLoc() {
        if( locator ) ((PquMap*)locator)->erase( compact(vertex->x, vertex->y) );
        this->loc = -1;
        this->locator = nullptr;
    }

    void setLocator( void* locator ) {
        this->locator = locator;
    }

    PquKey &operator=( PquKey &&other ) {
        vertex = other.vertex;
        other.vertex = nullptr;
        if( locator ) (*(PquMap*)locator)[compact(vertex->x, vertex->y)] = this->loc;
        return *this;
    }
};

struct PquKeyCmp {
    bool operator()( const PquKey &a, const PquKey &b ) const { return a.vertex->dist > b.vertex->dist; }
};

typedef AllocatorTracker< PquKey > PquTracker;

typedef robot::PriorityQueueU< PquKey, std::deque< PquKey, PquTracker >, PquKeyCmp > PquHeap;

void testPriorityQueueU() {

    PquTracker tracker;

    PquMap pquMap;

    PquHeap distQ( &pquMap, tracker );

    std::vector< int > v;
    int last = -1;

    for( int i = 0; i < 10; ++i ) {
        last = intRand( randEngine );
        distQ.emplace( vertexes + last );
        v.emplace_back( last );
    }

//    printf( "erase %f\n", vertexes[last].dist );
//    distQ.erase( PquKey( vertexes + last ) );

    float delta = 100.f;
    printf( "update %f -> %f\n", vertexes[last].dist, vertexes[last].dist + delta );
    for( auto it : pquMap ) printf( "%d %d\n", it.first, it.second );
    vertexes[last].dist += delta;
    distQ.update( vertexes + last );

    printf( "erase %f\n", vertexes[0].dist );
    distQ.erase( vertexes + 0 );

    while( distQ.size() > 0 ) {
        printf( "read %p = %f\n", distQ.top().vertex, distQ.top().vertex->dist );
        distQ.pop();
    }

    for( int i = 0; i < TESTN; ++i ) {
        distQ.emplace( vertexes + intRand( randEngine ) );
    }

    for( int i = 0; i < TESTN; ++i ) {
        auto key = distQ.top();
        auto& v = *(key.vertex);
        v.dist -= smallFloatRand( randEngine );
        distQ.update( &v );
    }

    while( distQ.size() > 0 ) {
        distQ.pop();
    }

    printf( " =============== PquHeap Begin =============\n" );

    printf( "HeapSize: %zu\n", distQ.size() );
    printf( "HeapMemory: %lld %lld\n", tracker.get_cur_size(), tracker.get_peak_size() );
    printf( "MapMemory: %lld %lld\n", pquMap.get_allocator().get_cur_size(), pquMap.get_allocator().get_peak_size() );
    printf( "MapSize: %zu\n", pquMap.size() );
    // printf( "MapMemory: %lld %lld\n", pqxHandleMap.get_allocator().get_cur_size(), pqxHandleMap.get_allocator().get_peak_size() );

    printf( " =============== PquHeap End =============\n" );
}

void init() {

}

struct Say {
    ~Say() {
        printf( "Say Des\n" );
    }
};

int main() {

    init();

    Benchmark bm;

    testDup();
    auto fibTime = bm.run_once( [](){ testFibonacci(); } );
    auto pqxTime = bm.run_once( [](){ testPriorityQueueX(); } );
    auto pquTime = bm.run_once( [](){ testPriorityQueueU(); } );

    printf( "FibTime: %lld\n", fibTime );
    printf( "PqxTime: %lld\n", pqxTime );
    printf( "PquTime: %lld\n", pquTime );

    return 0;
}
