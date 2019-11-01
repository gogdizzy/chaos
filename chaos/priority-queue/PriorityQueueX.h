//
// Created by 关鑫 on 2019-05-30.
//

#pragma once

#include <cstdio>
#include <algorithm>
#include <vector>

namespace robot {

template <class _Compare, class _RandomAccessIterator>
void
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

template <class _Compare, class _RandomAccessIterator>
void
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

template < class _Tp, class _LocFunc, class _Container = std::vector< _Tp >,
        class _Compare = std::less< typename _Container::value_type > >
class  PriorityQueueX {
public:
    typedef _LocFunc locator;
    typedef _Container container_type;
    typedef _Compare value_compare;
    typedef typename container_type::value_type value_type;
    typedef typename container_type::reference reference;
    typedef typename container_type::const_reference const_reference;
    typedef typename container_type::size_type size_type;
    static_assert( ( std::is_same< _Tp, value_type >::value ), "" );

protected:
    container_type c;
    value_compare comp;
    locator loc; // 根据key 定位 key包含的数据对应的handle地址

public:
    
    PriorityQueueX() : c(), comp() {}

    
    PriorityQueueX( const PriorityQueueX &__q ) : c( __q.c ), comp( __q.comp ) {}

    
    PriorityQueueX &operator=( const PriorityQueueX &__q ) {
        c = __q.c;
        comp = __q.comp;
        return *this;
    }

#ifndef _LIBCPP_CXX03_LANG

    
    PriorityQueueX( PriorityQueueX &&__q ) : c( std::move( __q.c ) ), comp( std::move( __q.comp ) ) {}

    
    PriorityQueueX &operator=( PriorityQueueX &&__q ) {
        c = std::move( __q.c );
        comp = std::move( __q.comp );
        return *this;
    }

#endif  // _LIBCPP_CXX03_LANG

    
    explicit PriorityQueueX( const value_compare &__comp )
            : c(), comp( __comp ) {}

    
    PriorityQueueX( const value_compare &__comp, const container_type &__c );

#ifndef _LIBCPP_CXX03_LANG

    
    explicit PriorityQueueX( const value_compare &__comp, container_type &&__c );

#endif

    template < class _InputIter >
    
    PriorityQueueX( _InputIter __f, _InputIter __l,
                    const value_compare &__comp = value_compare() );

    template < class _InputIter >
    
    PriorityQueueX( _InputIter __f, _InputIter __l,
                    const value_compare &__comp, const container_type &__c );

#ifndef _LIBCPP_CXX03_LANG

    template < class _InputIter >
    
    PriorityQueueX( _InputIter __f, _InputIter __l,
                    const value_compare &__comp, container_type &&__c );

#endif  // _LIBCPP_CXX03_LANG

    template < class _Alloc >
    
    explicit PriorityQueueX( const _Alloc &__a,
                             typename std::enable_if< std::uses_allocator< container_type,
                                     _Alloc >::value >::type * = 0 );

    template < class _Alloc >
    
    PriorityQueueX( const value_compare &__comp, const _Alloc &__a,
                    typename std::enable_if< std::uses_allocator< container_type,
                            _Alloc >::value >::type * = 0 );

    template < class _Alloc >
    
    PriorityQueueX( const value_compare &__comp, const container_type &__c,
                    const _Alloc &__a,
                    typename std::enable_if< std::uses_allocator< container_type,
                            _Alloc >::value >::type * = 0 );

    template < class _Alloc >
    
    PriorityQueueX( const PriorityQueueX &__q, const _Alloc &__a,
                    typename std::enable_if< std::uses_allocator< container_type,
                            _Alloc >::value >::type * = 0 );

#ifndef _LIBCPP_CXX03_LANG

    template < class _Alloc >
    
    PriorityQueueX( const value_compare &__comp, container_type &&__c,
                    const _Alloc &__a,
                    typename std::enable_if< std::uses_allocator< container_type,
                            _Alloc >::value >::type * = 0 );

    template < class _Alloc >
    
    PriorityQueueX( PriorityQueueX &&__q, const _Alloc &__a,
                    typename std::enable_if< std::uses_allocator< container_type,
                            _Alloc >::value >::type * = 0 );

#endif  // _LIBCPP_CXX03_LANG

    bool empty() const { return c.empty(); }

    size_type size() const { return c.size(); }

    
    const_reference top() const { return c.front(); }

    
    void push( const value_type &__v );

#ifndef _LIBCPP_CXX03_LANG

    
    void push( value_type &&__v );

    
    void erase( value_type &&__v );

    
    void update( value_type &&__v );

    template < class... _Args >
    
    void emplace( _Args &&... __args );

#endif  // _LIBCPP_CXX03_LANG

    
    void pop();

    
    void swap( PriorityQueueX &__q );
};

template < class _Tp, class _LocFunc, class _Container, class _Compare >
inline
PriorityQueueX< _Tp, _LocFunc, _Container, _Compare >::PriorityQueueX( const _Compare &__comp,
                                                             const container_type &__c )
        : c( __c ),
          comp( __comp ) {
    std::make_heap( c.begin(), c.end(), comp );
}

#ifndef _LIBCPP_CXX03_LANG

template < class _Tp, class _LocFunc, class _Container, class _Compare >
inline
PriorityQueueX< _Tp, _LocFunc, _Container, _Compare >::PriorityQueueX( const value_compare &__comp,
                                                             container_type &&__c )
        : c( std::move( __c ) ),
          comp( __comp ) {
    std::make_heap( c.begin(), c.end(), comp );
}

#endif  // _LIBCPP_CXX03_LANG

template < class _Tp, class _LocFunc, class _Container, class _Compare >
template < class _InputIter >
inline
PriorityQueueX< _Tp, _LocFunc, _Container, _Compare >::PriorityQueueX( _InputIter __f, _InputIter __l,
                                                             const value_compare &__comp )
        : c( __f, __l ),
          comp( __comp ) {
    std::make_heap( c.begin(), c.end(), comp );
}

template < class _Tp, class _LocFunc, class _Container, class _Compare >
template < class _InputIter >
inline
PriorityQueueX< _Tp, _LocFunc, _Container, _Compare >::PriorityQueueX( _InputIter __f, _InputIter __l,
                                                             const value_compare &__comp,
                                                             const container_type &__c )
        : c( __c ),
          comp( __comp ) {
    c.insert( c.end(), __f, __l );
    std::make_heap( c.begin(), c.end(), comp );
}

#ifndef _LIBCPP_CXX03_LANG

template < class _Tp, class _LocFunc, class _Container, class _Compare >
template < class _InputIter >
inline
PriorityQueueX< _Tp, _LocFunc, _Container, _Compare >::PriorityQueueX( _InputIter __f, _InputIter __l,
                                                             const value_compare &__comp,
                                                             container_type &&__c )
        : c( std::move( __c ) ),
          comp( __comp ) {
    c.insert( c.end(), __f, __l );
    std::make_heap( c.begin(), c.end(), comp );
}

#endif  // _LIBCPP_CXX03_LANG

template < class _Tp, class _LocFunc, class _Container, class _Compare >
template < class _Alloc >
inline
PriorityQueueX< _Tp, _LocFunc, _Container, _Compare >::PriorityQueueX( const _Alloc &__a,
                                                             typename std::enable_if< std::uses_allocator< container_type,
                                                                     _Alloc >::value >::type * )
        : c( __a ) {
}

template < class _Tp, class _LocFunc, class _Container, class _Compare >
template < class _Alloc >
inline
PriorityQueueX< _Tp, _LocFunc, _Container, _Compare >::PriorityQueueX( const value_compare &__comp,
                                                             const _Alloc &__a,
                                                             typename std::enable_if< std::uses_allocator< container_type,
                                                                     _Alloc >::value >::type * )
        : c( __a ),
          comp( __comp ) {
}

template < class _Tp, class _LocFunc, class _Container, class _Compare >
template < class _Alloc >
inline
PriorityQueueX< _Tp, _LocFunc, _Container, _Compare >::PriorityQueueX( const value_compare &__comp,
                                                             const container_type &__c,
                                                             const _Alloc &__a,
                                                             typename std::enable_if< std::uses_allocator< container_type,
                                                                     _Alloc >::value >::type * )
        : c( __c, __a ),
          comp( __comp ) {
    std::make_heap( c.begin(), c.end(), comp );
}

template < class _Tp, class _LocFunc, class _Container, class _Compare >
template < class _Alloc >
inline
PriorityQueueX< _Tp, _LocFunc, _Container, _Compare >::PriorityQueueX( const PriorityQueueX &__q,
                                                             const _Alloc &__a,
                                                             typename std::enable_if< std::uses_allocator< container_type,
                                                                     _Alloc >::value >::type * )
        : c( __q.c, __a ),
          comp( __q.comp ) {
    std::make_heap( c.begin(), c.end(), comp );
}

#ifndef _LIBCPP_CXX03_LANG

template < class _Tp, class _LocFunc, class _Container, class _Compare >
template < class _Alloc >
inline
PriorityQueueX< _Tp, _LocFunc, _Container, _Compare >::PriorityQueueX( const value_compare &__comp,
                                                             container_type &&__c,
                                                             const _Alloc &__a,
                                                             typename std::enable_if< std::uses_allocator< container_type,
                                                                     _Alloc >::value >::type * )
        : c( std::move( __c ), __a ),
          comp( __comp ) {
    std::make_heap( c.begin(), c.end(), comp );
}

template < class _Tp, class _LocFunc, class _Container, class _Compare >
template < class _Alloc >
inline
PriorityQueueX< _Tp, _LocFunc, _Container, _Compare >::PriorityQueueX( PriorityQueueX &&__q,
                                                             const _Alloc &__a,
                                                             typename std::enable_if< std::uses_allocator< container_type,
                                                                     _Alloc >::value >::type * )
        : c( std::move( __q.c ), __a ),
          comp( std::move( __q.comp ) ) {
    std::make_heap( c.begin(), c.end(), comp );
}

#endif  // _LIBCPP_CXX03_LANG

template < class _Tp, class _LocFunc, class _Container, class _Compare >
inline
void
PriorityQueueX< _Tp, _LocFunc, _Container, _Compare >::push( const value_type &__v ) {
    c.push_back( __v );
    loc( *c.rbegin() ) = c.rbegin()->loc = c.size() - 1;
    std::push_heap( c.begin(), c.end(), comp );
}

#ifndef _LIBCPP_CXX03_LANG

template < class _Tp, class _LocFunc, class _Container, class _Compare >
inline
void
PriorityQueueX< _Tp, _LocFunc, _Container, _Compare >::push( value_type &&__v ) {
    c.push_back( std::move( __v ) );
    loc( *c.rbegin() ) = c.rbegin()->loc = c.size() - 1;
    std::push_heap( c.begin(), c.end(), comp );
}

template < class _Tp, class _LocFunc, class _Container, class _Compare >
inline
void
PriorityQueueX< _Tp, _LocFunc, _Container, _Compare >::erase( value_type &&__v ) {
    int pos = loc( __v );
    if( pos + 1 == c.size() ) {
        c.pop_back();
        return;
    }

    c[pos] = std::move( c.back() );
    c.pop_back();
    sift_up( c.begin(), c.begin() + pos + 1, comp, pos + 1 );
    sift_down( c.begin(), c.end(), comp, c.end() - c.begin(), c.begin() + pos );
}

template < class _Tp, class _LocFunc, class _Container, class _Compare >
inline
void
PriorityQueueX< _Tp, _LocFunc, _Container, _Compare >::update( value_type &&__v ) {
    int pos = loc( __v );
    sift_up( c.begin(), c.begin() + pos + 1, comp, pos + 1 );
    sift_down( c.begin(), c.end(), comp, c.end() - c.begin(), c.begin() + pos );
}

template < class _Tp, class _LocFunc, class _Container, class _Compare >
template < class... _Args >
inline
void
PriorityQueueX< _Tp, _LocFunc, _Container, _Compare >::emplace( _Args &&... __args ) {

    c.emplace_back( std::forward< _Args >( __args )... );
    loc( *c.rbegin() ) = c.rbegin()->loc = c.size() - 1;
    std::push_heap( c.begin(), c.end(), comp );
}

#endif  // _LIBCPP_CXX03_LANG

template < class _Tp, class _LocFunc, class _Container, class _Compare >
inline
void
PriorityQueueX< _Tp, _LocFunc, _Container, _Compare >::pop() {
    std::pop_heap( c.begin(), c.end(), comp );
    c.pop_back();
}

template < class _Tp, class _LocFunc, class _Container, class _Compare >
inline
void
PriorityQueueX< _Tp, _LocFunc, _Container, _Compare >::swap( PriorityQueueX &__q ) {
    using std::swap;
    swap( c, __q.c );
    swap( comp, __q.comp );
}

template < class _Tp, class _LocFunc, class _Container, class _Compare >
inline 
void
swap( PriorityQueueX< _Tp, _LocFunc, _Container, _Compare > &__x,
      PriorityQueueX< _Tp, _LocFunc, _Container, _Compare > &__y )
_NOEXCEPT_( _NOEXCEPT_( __x.swap( __y ) ) ) {
    __x.swap( __y );
}

}