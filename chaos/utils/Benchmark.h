//
// Created by 关鑫 on 2019-02-02.
//

#pragma once

#include <cstdio>
#include <functional>

class Benchmark {
public:
    Benchmark( int catalog_width = 16, int item_width = 16 )
        : catalog_width_( catalog_width ),
          item_width_( item_width ) {
    }

    void head( const char *catalog, std::initializer_list< const char * > heads ) {
        printf( "%*s", catalog_width, catalog );
        for( auto h : heads ) {
            printf( "%*s", item_width, h );
        }
        printf( "\n" );
    }

    template< typename Func >
    void content( const char *catalog, std::initializer_list< const Func > funcs ) {
        printf( "%*s", catalog_width, catalog );
        for( auto &f : funcs ) {
            auto beg = std::chrono::high_resolution_clock::now();
            f();
            auto end = std::chrono::high_resolution_clock::now();
            printf( "%*lld", item_width, std::chrono::duration_cast< std::chrono::microseconds >(end - beg).count() );
        }
        printf( "\n" );
    }

private:
    int catalog_width_;
    int item_width_;
};