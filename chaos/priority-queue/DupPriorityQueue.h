//
// Created by 关鑫 on 2019-05-30.
//

#pragma once

#include <queue>

template< class _Tp, class _Container = vector<_Tp>,
        class _Compare = less<typename _Container::value_type> >
class DupPriorityQueue {

public:


private:
    std::priority_queue< _Tp, _Container, _Compare > pq_;
};
