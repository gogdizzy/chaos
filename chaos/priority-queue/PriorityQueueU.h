//
// Created by 关鑫 on 2019-05-30.
//

#pragma once

#include <cstdio>
#include <algorithm>
#include <vector>

namespace robot {

struct PquKeyBase {
    int loc;
    void *locator;

    PquKeyBase() : loc( -1 ), locator( nullptr ) {}

    PquKeyBase( const PquKeyBase &other ) : loc( other.loc ), locator( other.locator ) {}

    PquKeyBase( PquKeyBase &&other ) : loc( std::move( other.loc ) ), locator( std::move( other.locator ) ) {}
};

template < class _Tp, class _Container = std::vector< _Tp >,
        class _Compare = std::less< typename _Container::value_type > >
class PriorityQueueU {
public:
    typedef _Container container_type;
    typedef _Compare value_compare;
    typedef typename container_type::value_type value_type;
    typedef typename container_type::reference reference;
    typedef typename container_type::const_reference const_reference;
    typedef typename container_type::size_type size_type;
    typedef typename container_type::allocator_type heap_alloc_type;
    static_assert( ( std::is_same< _Tp, value_type >::value ), "" );

protected:
    container_type c;
    value_compare comp;
    void *locator; // 根据key 定位 key包含的数据对应的handle地址

public:

    PriorityQueueU() : c(), comp(), locator( nullptr ) {}

    PriorityQueueU( const PriorityQueueU &__q ) : c( __q.c ), comp( __q.comp ), locator( __q.locator ) {}

    PriorityQueueU &operator=( const PriorityQueueU &__q ) {
        c = __q.c;
        comp = __q.comp;
        locator = __q.locator;
        return *this;
    }

    PriorityQueueU( PriorityQueueU &&__q ) : c( std::move( __q.c ) ), comp( std::move( __q.comp ) ), locator( std::move( __q.locator ) ) {}

    PriorityQueueU &operator=( PriorityQueueU &&__q ) {
        c = std::move( __q.c );
        comp = std::move( __q.comp );
        locator = __q.locator;
        return *this;
    }

    explicit
    PriorityQueueU( void* __locator )
            : c(), comp(), locator( __locator ) {}

    template< typename _Alloc >
    explicit
    PriorityQueueU( void* __locator, const _Alloc &__a )
            : c( __a ), locator( __locator ) {}

    bool empty() const { return c.empty(); }

    size_type size() const { return c.size(); }

    const_reference top() const { return c.front(); }

    void push( const value_type &__v );

    void push( value_type &&__v );

    void erase( value_type &__v );

    void erase( value_type &&__v );

    void update( value_type &__v );

    void update( value_type &&__v );

    template < class... _Args >
    void emplace( _Args &&... __args );

    void pop();

    void swap( PriorityQueueU &__q );

    void clear();

    bool contains( value_type &&__v );

protected:

    template <class _RandomAccessIterator>
    static void
    sift_up(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp,
            typename std::iterator_traits<_RandomAccessIterator>::difference_type __len)
    {
        typedef typename std::iterator_traits<_RandomAccessIterator>::value_type value_type;
        if (__len > 1)
        {
            __len = (__len - 2) / 2;
            _RandomAccessIterator __ptr = __first + __len;
            if (__comp(*__ptr, *--__last))
            {
                value_type __t(std::move(*__last));
                do
                {
                    *__last = std::move(*__ptr);
                    __last = __ptr;
                    if (__len == 0)
                        break;
                    __len = (__len - 1) / 2;
                    __ptr = __first + __len;
                } while (__comp(*__ptr, __t));
                *__last = std::move(__t);
            }
        }
    }

    template <class _RandomAccessIterator>
    static void
    sift_down(_RandomAccessIterator __first, _RandomAccessIterator /*__last*/,
              _Compare __comp,
              typename std::iterator_traits<_RandomAccessIterator>::difference_type __len,
              _RandomAccessIterator __start)
    {
        typedef typename std::iterator_traits<_RandomAccessIterator>::difference_type difference_type;
        typedef typename std::iterator_traits<_RandomAccessIterator>::value_type value_type;
        // left-child of __start is at 2 * __start + 1
        // right-child of __start is at 2 * __start + 2
        difference_type __child = __start - __first;

        if (__len < 2 || (__len - 2) / 2 < __child)
            return;

        __child = 2 * __child + 1;
        _RandomAccessIterator __child_i = __first + __child;

        if ((__child + 1) < __len && __comp(*__child_i, *(__child_i + 1))) {
            // right-child exists and is greater than left-child
            ++__child_i;
            ++__child;
        }

        // check if we are in heap-order
        if (__comp(*__child_i, *__start))
            // we are, __start is larger than it's largest child
            return;

        value_type __top(std::move(*__start));
        do
        {
            // we are not in heap-order, swap the parent with it's largest child
            *__start = std::move(*__child_i);
            __start = __child_i;

            if ((__len - 2) / 2 < __child)
                break;

            // recompute the child based off of the updated parent
            __child = 2 * __child + 1;
            __child_i = __first + __child;

            if ((__child + 1) < __len && __comp(*__child_i, *(__child_i + 1))) {
                // right-child exists and is greater than left-child
                ++__child_i;
                ++__child;
            }

            // check if we are in heap-order
        } while (!__comp(*__child_i, __top));
        *__start = std::move(__top);
    }
};


template < class _Tp, class _Container, class _Compare >
inline
void
PriorityQueueU< _Tp, _Container, _Compare >::push( const value_type &__v ) {
    c.push_back( __v );
    auto &key = *c.rbegin();
    key.setLocator( locator );
    key.setLoc( c.size() - 1 );
    std::push_heap( c.begin(), c.end(), comp );
}

template < class _Tp, class _Container, class _Compare >
inline
void
PriorityQueueU< _Tp, _Container, _Compare >::push( value_type &&__v ) {
    c.push_back( std::move( __v ) );
    auto &key = *c.rbegin();
    key.setLocator( locator );
    key.setLoc( c.size() - 1 );
    std::push_heap( c.begin(), c.end(), comp );
}

template < class _Tp, class _Container, class _Compare >
inline
void
PriorityQueueU< _Tp, _Container, _Compare >::erase( value_type &__v ) {
    __v.setLocator( locator );
    int pos = __v.getLoc();
    if( pos < 0 || pos >= c.size() ) return;
    if( pos + 1 == c.size() ) {
        c.pop_back();
        __v.eraseLoc();
        return;
    }

    c[pos] = std::move( c.back() );
    c.pop_back();
    sift_up( c.begin(), c.begin() + pos + 1, comp, pos + 1 );
    sift_down( c.begin(), c.end(), comp, c.end() - c.begin(), c.begin() + pos );

    __v.eraseLoc();
}

template < class _Tp, class _Container, class _Compare >
inline
void
PriorityQueueU< _Tp, _Container, _Compare >::erase( value_type &&__v ) {
    __v.setLocator( locator );
    int pos = __v.getLoc();
    if( pos < 0 || pos >= c.size() ) return;
    if( pos + 1 == c.size() ) {
        c.pop_back();
        __v.eraseLoc();
        return;
    }

    c[pos] = std::move( c.back() );
    c.pop_back();
    sift_up( c.begin(), c.begin() + pos + 1, comp, pos + 1 );
    sift_down( c.begin(), c.end(), comp, c.end() - c.begin(), c.begin() + pos );

    __v.eraseLoc();
}

template < class _Tp, class _Container, class _Compare >
inline
void
PriorityQueueU< _Tp, _Container, _Compare >::update( value_type &__v ) {
    __v.setLocator( locator );
    int pos = __v.getLoc();
    c[pos] = std::move( __v );
    sift_up( c.begin(), c.begin() + pos + 1, comp, pos + 1 );
    sift_down( c.begin(), c.end(), comp, c.end() - c.begin(), c.begin() + pos );
}

template < class _Tp, class _Container, class _Compare >
inline
void
PriorityQueueU< _Tp, _Container, _Compare >::update( value_type &&__v ) {
    __v.setLocator( locator );
    int pos = __v.getLoc();
    c[pos] = std::move( __v );
    sift_up( c.begin(), c.begin() + pos + 1, comp, pos + 1 );
    sift_down( c.begin(), c.end(), comp, c.end() - c.begin(), c.begin() + pos );
}

template < class _Tp, class _Container, class _Compare >
template < class... _Args >
inline
void
PriorityQueueU< _Tp, _Container, _Compare >::emplace( _Args &&... __args ) {

    c.emplace_back( std::forward< _Args >( __args )... );
    auto &key = *c.rbegin();
    key.setLocator( locator );
    key.setLoc( c.size() - 1 );
    std::push_heap( c.begin(), c.end(), comp );
}

template < class _Tp, class _Container, class _Compare >
inline
void
PriorityQueueU< _Tp, _Container, _Compare >::pop() {
    std::pop_heap( c.begin(), c.end(), comp );
    ( *c.rbegin() ).eraseLoc();
    c.pop_back();
}

template < class _Tp, class _Container, class _Compare >
inline
void
PriorityQueueU< _Tp, _Container, _Compare >::swap( PriorityQueueU &__q ) {
    using std::swap;
    swap( c, __q.c );
    swap( comp, __q.comp );
    swap( locator, __q.locator );
}

template < class _Tp, class _Container, class _Compare >
inline
void
swap( PriorityQueueU< _Tp, _Container, _Compare > &__x,
      PriorityQueueU< _Tp, _Container, _Compare > &__y ) {
    __x.swap( __y );
}

template < class _Tp, class _Container, class _Compare >
inline
void
PriorityQueueU< _Tp, _Container, _Compare >::clear() {
    for( auto &key : c ) key.eraseLoc();
    c.clear();
}

template < class _Tp, class _Container, class _Compare >
inline
bool
PriorityQueueU< _Tp, _Container, _Compare >::contains( value_type &&__v ) {
    __v.setLocator( locator );
    return __v.getLoc() != -1;
}

}