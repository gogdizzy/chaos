/*
 * SparseGridMap.h
 *
 *  Created on: Jan 8, 2015
 *      Author: william
 */

#ifndef NAVUTILS_SPARSEGRIDMAP_H_
#define NAVUTILS_SPARSEGRIDMAP_H_

#include <unordered_map>
#include <functional>

template <typename CellType>
class SparseGridMap
{
public:
	SparseGridMap();
	SparseGridMap(CellType defaultVal);
	SparseGridMap(const SparseGridMap<CellType>& copiedObj);
	SparseGridMap<CellType>& operator=(const SparseGridMap<CellType>& assignedObj);
	virtual ~SparseGridMap();


	CellType Get(const int32_t& x, const int32_t& y) const;
	void Set(const int32_t& x, const int32_t& y, const CellType& val);
	void Remove(const int32_t& x, const int32_t& y);
	bool Contains(const int32_t& x, const int32_t& y);
	bool IsEmpty();
	void Clear();

	template <typename Fun>
	void ForEach(const Fun& fun) const;

	// It may create a node when calling this function, suggest use Get(), Set() instead
	CellType& At(const int32_t& x, const int32_t& y);

private:
	std::unordered_map<int32_t, std::unordered_map<int32_t, CellType> > _map;
	bool _bHasDefaultVal;
	CellType _defaultVal;
};


template <typename CellType>
SparseGridMap<CellType>::SparseGridMap() : _bHasDefaultVal(false)
{
}

template <typename CellType>
SparseGridMap<CellType>::SparseGridMap(CellType defaultVal):
	_bHasDefaultVal(true), _defaultVal(defaultVal)
{
}

template <typename CellType>
SparseGridMap<CellType>::SparseGridMap(const SparseGridMap<CellType>& copiedObj)
{
    this->_bHasDefaultVal = copiedObj._bHasDefaultVal;
    this->_defaultVal = copiedObj._defaultVal;
    this->_map = copiedObj._map;
}

template <typename CellType>
SparseGridMap<CellType>& SparseGridMap<CellType>::operator=(const SparseGridMap<CellType>& assignedObj)
{
    if(this == &assignedObj)
    {
        return *this;
    }

    this->_bHasDefaultVal = assignedObj._bHasDefaultVal;
    this->_defaultVal = assignedObj._defaultVal;
    this->_map = assignedObj._map;
    return *this;
}

template <typename CellType>
SparseGridMap<CellType>::~SparseGridMap()
{
}

template <typename CellType>
CellType& SparseGridMap<CellType>::At(const int32_t& x, const int32_t& y)
{
	if (_bHasDefaultVal && _map[y].count(x) == 0)
	{
		_map[y][x] = _defaultVal;
	}

	return _map[y][x];
}
template <typename CellType>
CellType SparseGridMap<CellType>::Get(const int32_t& x, const int32_t& y) const
{
	auto iterY = _map.find(y);
	if (iterY != _map.end())
	{
		auto iterX = iterY->second.find(x);
		if (iterX != iterY->second.end())
			return iterX->second;
	}

	return _defaultVal;
}

template <typename CellType>
void SparseGridMap<CellType>::Set(const int32_t& x, const int32_t& y, const CellType& val)
{
	if (_bHasDefaultVal && val == _defaultVal)
	{
		Remove(x, y);
	}
	else
	{
		_map[y][x] = val;
	}
}
template <typename CellType>
void SparseGridMap<CellType>::Remove(const int32_t& x, const int32_t& y)
{
	auto iterY = _map.find(y);
	if (iterY != _map.end())
	{
		auto iterX = iterY->second.find(x);
		if (iterX != iterY->second.end())
			iterY->second.erase(iterX);
		if(iterY->second.size() == 0)
			_map.erase(iterY);
	}
}

template <typename CellType>
bool SparseGridMap<CellType>::Contains(const int32_t& x, const int32_t& y)
{
	auto iterY = _map.find(y);
	if (iterY != _map.end())
	{
		auto iterX = iterY->second.find(x);
		if (iterX != iterY->second.end())
			return true;
	}
	return false;
}

template <typename CellType>
bool SparseGridMap<CellType>::IsEmpty()
{
    return _map.empty();
}

template <typename CellType>
void SparseGridMap<CellType>::Clear()
{
	_map.clear();
}

template <typename CellType>
template <typename Fun>
void  SparseGridMap<CellType>::ForEach(const Fun& handle) const
{
	for (auto iterY = _map.begin(); iterY != _map.end(); iterY++)
	{
		for (auto iterX = iterY->second.begin(); iterX != iterY->second.end(); iterX++)
		{
			handle(iterX->first, iterY->first, iterX->second);
		}
	}
}

#endif /* NAVUTILS_SPARSEGRIDMAP_H_ */
