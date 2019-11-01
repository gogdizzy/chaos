//
// Created by 关鑫 on 2019-08-01.
//

#pragma once

#include <unordered_map>
#include <functional>

template < typename CellType >
class SparseGridMap
{
public:
    SparseGridMap();

    SparseGridMap( CellType defaultVal );

    SparseGridMap( const SparseGridMap< CellType > &copiedObj );

    SparseGridMap< CellType > &operator=( const SparseGridMap< CellType > &assignedObj );

    virtual ~SparseGridMap();

    CellType Get( const int32_t x, const int32_t y ) const;

    void Set( const int32_t x, const int32_t y, const CellType &val );

    void SetDefault( const CellType &val );

    void Remove( const int32_t x, const int32_t y );

    bool Contains( const int32_t x, const int32_t y );

    bool IsEmpty();

    void Clear();

    template < typename Fun >
    void ForEach( const Fun &fun ) const;

    // It may create a node when calling this function, suggest use Get(), Set() instead
    CellType &At( const int32_t x, const int32_t y );

    inline size_t Size() { return _map.size(); }

private:
    std::unordered_map< int32_t, CellType > _map;
    bool _bHasDefaultVal;
    CellType _defaultVal;

    static inline int32_t GetIndex( const int32_t x, const int32_t y )
    {
        return ( x << 16 ) | ( y & 0xFFFF );
    }
};


template < typename CellType >
SparseGridMap< CellType >::SparseGridMap() : _bHasDefaultVal( false )
{
}

template < typename CellType >
SparseGridMap< CellType >::SparseGridMap( CellType defaultVal ):
        _bHasDefaultVal( true ), _defaultVal( defaultVal )
{
}

template < typename CellType >
SparseGridMap< CellType >::SparseGridMap( const SparseGridMap< CellType > &copiedObj )
{
    this->_bHasDefaultVal = copiedObj._bHasDefaultVal;
    this->_defaultVal = copiedObj._defaultVal;
    this->_map = copiedObj._map;
}

template < typename CellType >
SparseGridMap< CellType > &SparseGridMap< CellType >::operator=( const SparseGridMap< CellType > &assignedObj )
{
    if( this == &assignedObj )
    {
        return *this;
    }

    this->_bHasDefaultVal = assignedObj._bHasDefaultVal;
    this->_defaultVal = assignedObj._defaultVal;
    this->_map = assignedObj._map;
    return *this;
}

template < typename CellType >
SparseGridMap< CellType >::~SparseGridMap()
{
}

template < typename CellType >
CellType &SparseGridMap< CellType >::At( const int32_t x, const int32_t y )
{
    int32_t index = GetIndex( x, y );
    auto it = _map.find( index );
    if( it != _map.end() ) return it->second;
    if( _bHasDefaultVal ) _map.emplace( index, _defaultVal );
    return _map[index];
}

template < typename CellType >
CellType SparseGridMap< CellType >::Get( const int32_t x, const int32_t y ) const
{
    int32_t index = GetIndex( x, y );
    auto it = _map.find( index );
    if( it != _map.end() ) return it->second;
    return _defaultVal;
}

template < typename CellType >
void SparseGridMap< CellType >::Set( const int32_t x, const int32_t y, const CellType &val )
{
    if( _bHasDefaultVal && val == _defaultVal )
    {
        Remove( x, y );
    }
    else
    {
        _map[GetIndex( x, y )] = val;
    }
}

template < typename CellType >
void SparseGridMap< CellType >::SetDefault( const CellType &val )
{
    _bHasDefaultVal = true;
    _defaultVal = val;
}

template < typename CellType >
void SparseGridMap< CellType >::Remove( const int32_t x, const int32_t y )
{
    _map.erase( GetIndex( x, y ) );
}

template < typename CellType >
bool SparseGridMap< CellType >::Contains( const int32_t x, const int32_t y )
{
    return _map.find( GetIndex( x, y ) ) != _map.end();
}

template < typename CellType >
bool SparseGridMap< CellType >::IsEmpty()
{
    return _map.empty();
}

template < typename CellType >
void SparseGridMap< CellType >::Clear()
{
    _map.clear();
}

template < typename CellType >
template < typename Fun >
void SparseGridMap< CellType >::ForEach( const Fun &handle ) const
{
    for( const auto& it : _map )
    {
        int32_t index = it.first;
        int32_t x = ( index >> 16 );
        int32_t y = (int16_t) ( index & 0xFFFF );
        handle( x, y, it.second );
    }
}


