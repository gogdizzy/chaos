//
// Created by 关鑫 on 2020/1/17.
//

#include <stdint.h>
#include <stdio.h>
#include "UnionSet.h"
#include <array>

int main() {

    std::array<int, 10> sa;

    UnionSet<int16_t, 10> us1;

    us1.Merge(1, 2);
    us1.Merge(1, 3);
    us1.Merge(1, 4);
    us1.Merge(5, 4);
    us1.Merge(6, 3);

    for( int i = 0; i < 10; ++i ) printf( "%d ", (int)us1.GetRoot(i) );
    printf("\n");

    UnionSet<int16_t, 10> us2(5);

    us2.Merge(-2, 2);
    us2.Merge(-1, 1);
    us2.Merge(1, 2);
    us2.Merge(2, 3);
    us2.Merge(0, 3);

    for( int i = -5; i < 5; ++i ) printf( "%d ", (int)us2.GetRoot(i) );
    printf("\n");

    us2.StatCount();

    for( int i = -5; i < 5; ++i ) printf( "%d ", (int)us2.GetCount(i) );
    printf("\n");

    return 0;
}