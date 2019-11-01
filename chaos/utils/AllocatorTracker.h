//
// Created by 关鑫 on 2019-05-30.
//
// Source: https://github.com/efficient/fast-succinct-trie.git

#pragma once

#include <cstdint>
#include <memory>

template < typename ValueType >
class AllocatorTracker : public std::allocator< ValueType > {
public:
    typedef typename std::allocator< ValueType > BaseAllocator;
    typedef typename BaseAllocator::pointer pointer;
    typedef typename BaseAllocator::size_type size_type;

    int64_t *cur_size;
    int64_t *peak_size;
    int64_t data[2];

    AllocatorTracker() throw() : BaseAllocator() {
        data[0] = data[1] = 0;
        cur_size = &data[0];
        peak_size = &data[1];
    }

    AllocatorTracker( int64_t *cur_ptr, int64_t *peak_ptr ) throw() : BaseAllocator() {
        cur_size = cur_ptr;
        peak_size = peak_ptr;
    }

    AllocatorTracker( const AllocatorTracker &allocator ) throw() : BaseAllocator( allocator ) {
        cur_size = allocator.cur_size;
        peak_size = allocator.peak_size;
    }

    template < class U >
    AllocatorTracker( const AllocatorTracker< U > &allocator ) throw(): BaseAllocator( allocator ) {
        cur_size = allocator.cur_size;
        peak_size = allocator.peak_size;
    }

    ~AllocatorTracker() {}

    template < class U >
    struct rebind {
        typedef AllocatorTracker< U > other;
    };

    pointer allocate( size_type size ) {
        pointer dataPtr = BaseAllocator::allocate( size );
        *cur_size += size * sizeof( ValueType );
        if( *cur_size > *peak_size ) *peak_size = *cur_size;
        return dataPtr;
    }

    pointer allocate( size_type size, void *ptr ) {
        pointer dataPtr = BaseAllocator::allocate( size, ptr );
        *cur_size += size * sizeof( ValueType );
        if( *cur_size > *peak_size ) *peak_size = *cur_size;
        return dataPtr;
    }

    pointer allocate( size_type size, pointer ptr ) {
        pointer dataPtr = BaseAllocator::allocate( size, ptr );
        *cur_size += size * sizeof( ValueType );
        if( *cur_size > *peak_size ) *peak_size = *cur_size;
        return dataPtr;
    }

    void deallocate( pointer ptr, size_type size ) throw() {
        BaseAllocator::deallocate( ptr, size );
        *cur_size -= size * sizeof( ValueType );
    }

    int64_t get_cur_size() {
        return *cur_size;
    }

    int64_t get_peak_size() {
        return *peak_size;
    }
};
