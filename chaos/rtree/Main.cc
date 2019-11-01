//
// Created by 关鑫 on 2019-08-01.
//

#include <vector>
#include <queue>
#include <random>

#include "utils/AllocatorTracker.h"
#include "utils/Benchmark.h"
#include "utils/SparseGridMap.h"

#include "Types.h"
#include "RStarTree.h"

#define BUBBLE_SIZE 5

#define MAXV 512

#define TESTN 12000

std::default_random_engine randEngine;
std::uniform_int_distribution< int > intRand( 0, MAXV - 1 );
std::uniform_real_distribution< float > floatRand( 0.f, 100.f );
std::uniform_real_distribution< float > smallFloatRand( 0.f, 10.f );

std::vector<ChangedPoint> pts;

template <typename T>
bool MergeRectByArea(SimpleRect<T>& leftRect, const SimpleRect<T>& rightRect)
{
    bool couldMerge = false;
    T intersectX, intersectY, intersectArea, spaceArea;
    bool hasvalue_intersectX = true, hasvalue_intersectY = true;

    if(leftRect.StX() >= rightRect.StX() && leftRect.StX() <= rightRect.EdX())
    {
        if(leftRect.EdX() <= rightRect.EdX())
        {
            intersectX = leftRect.Width();
        }
        else
        {
            intersectX = rightRect.EdX() - leftRect.StX();
        }
    }
    else if(leftRect.EdX() >= rightRect.StX() && leftRect.EdX() <= rightRect.EdX())
    {
        intersectX = leftRect.EdX() - rightRect.StX();
    }
    else if(leftRect.StX() <= rightRect.StX() && leftRect.EdX() >= rightRect.EdX())
    {
        intersectX = rightRect.Width();
    }
    else
    {
        hasvalue_intersectX = false;
    }

    if(leftRect.StY() >= rightRect.StY() && leftRect.StY() <= rightRect.EdY())
    {
        if(leftRect.EdY() <= rightRect.EdY())
        {
            intersectY = leftRect.Height();
        }
        else
        {
            intersectY = rightRect.EdY() - leftRect.StY();
        }
    }
    else if(leftRect.EdY() >= rightRect.StY() && leftRect.EdY() <= rightRect.EdY())
    {
        intersectY = leftRect.EdY() - rightRect.StY();
    }
    else if(leftRect.StY() <= rightRect.StY() && leftRect.EdY() >= rightRect.EdY())
    {
        intersectY = rightRect.Height();
    }
    else
    {
        hasvalue_intersectY = false;
    }

    if(hasvalue_intersectX && hasvalue_intersectY)
    {
        intersectArea = intersectX * intersectY;
        SimpleRect<T> tmpRect(SimplePoint<T>(std::min(leftRect.StX(), rightRect.StX()), std::min(leftRect.StY(), rightRect.StY())),
                              SimplePoint<T>(std::max(leftRect.EdX(), rightRect.EdX()), std::max(leftRect.EdY(), rightRect.EdY())));
        spaceArea = tmpRect.Width() * tmpRect.Height()
                    - (leftRect.Width() * leftRect.Height() + rightRect.Width() * rightRect.Height() - intersectArea);
        if(spaceArea <= intersectArea)
        {
            leftRect = tmpRect;
            couldMerge = true;
        }
    }
    return couldMerge;
}

template <typename T>
bool MergeRectByArea2(SimpleRect<T>& leftRect, const SimpleRect<T>& rightRect)
{
    if( leftRect.StX() > rightRect.EdX() || leftRect.StY() > rightRect.EdY() ||
        rightRect.StX() > leftRect.EdX() || rightRect.StY() > leftRect.EdY() ) {
        return false;
    }

    SimpleRect<T> boundRect(SimplePoint<T>(std::min(leftRect.StX(), rightRect.StX()), std::min(leftRect.StY(), rightRect.StY())),
                          SimplePoint<T>(std::max(leftRect.EdX(), rightRect.EdX()), std::max(leftRect.EdY(), rightRect.EdY())));
    if( leftRect.Area() + rightRect.Area() >= boundRect.Area() ) {
        leftRect = boundRect;
        return true;
    }
    return false;
}

std::vector<SimpleRect<int32_t> > GetChangedRectsFromChangedPoints(std::vector<ChangedPoint>& changedPoints, int bubbledSize)
{
    std::vector<SimpleRect<int32_t> > changedRects;
    std::queue<SimpleRect<int32_t> > changedRectQueue;
    SparseGridMap<ObstacleState> uniqueChangedPoints;

    for (ChangedPoint& point : changedPoints)
    {
        if (!uniqueChangedPoints.Contains(point.xNav, point.yNav))
        {
            changedRectQueue.push(SimpleRect<int32_t>(point.xNav - bubbledSize, point.yNav - bubbledSize, (int32_t)bubbledSize * 2, (int32_t)bubbledSize * 2));
            uniqueChangedPoints.Set(point.xNav, point.yNav, point.newState);
        }
    }

    while (changedRectQueue.size() > 0)
    {
        SimpleRect<int32_t> activeRect = changedRectQueue.front();
        changedRectQueue.pop();
        int changedRectQueueSize = changedRectQueue.size();
        bool couldMerge = false;
        for (int i = 0; i < changedRectQueueSize; i++)
        {
            SimpleRect<int32_t> rect = changedRectQueue.front();
            changedRectQueue.pop();
            if (MergeRectByArea(activeRect, rect))
            {
                couldMerge = true;
            }
            else
            {
                changedRectQueue.push(rect);
            }
        }
        if (couldMerge)
        {
            changedRectQueue.push(activeRect);
        }
        else
        {
            changedRects.push_back(activeRect);
        }
    }

    return changedRects;
}

std::vector<SimpleRect<int32_t> > GetChangedRectsFromChangedPoints2(std::vector<ChangedPoint>& changedPoints, int bubbledSize)
{
    std::vector<SimpleRect<int32_t> > changedRects;
    std::queue<SimpleRect<int32_t> > changedRectQueue;
    SparseGridMap<ObstacleState> uniqueChangedPoints;

    for (ChangedPoint& point : changedPoints)
    {
        if (!uniqueChangedPoints.Contains(point.xNav, point.yNav))
        {
            changedRectQueue.push(SimpleRect<int32_t>(point.xNav - bubbledSize, point.yNav - bubbledSize, (int32_t)bubbledSize * 2, (int32_t)bubbledSize * 2));
            uniqueChangedPoints.Set(point.xNav, point.yNav, point.newState);
        }
    }

    while (changedRectQueue.size() > 0)
    {
        SimpleRect<int32_t> activeRect = changedRectQueue.front();
        changedRectQueue.pop();
        int changedRectQueueSize = changedRectQueue.size();
        bool couldMerge = false;
        for (int i = 0; i < changedRectQueueSize; i++)
        {
            SimpleRect<int32_t> rect = changedRectQueue.front();
            changedRectQueue.pop();
            if (MergeRectByArea2(activeRect, rect))
            {
                couldMerge = true;
            }
            else
            {
                changedRectQueue.push(rect);
            }
        }
        if (couldMerge)
        {
            changedRectQueue.push(activeRect);
        }
        else
        {
            changedRects.push_back(activeRect);
        }
    }

    return changedRects;
}

std::vector<SimpleRect<int32_t> > GetChangedRectsFromChangedPoints3(std::vector<ChangedPoint>& changedPoints, int bubbledSize)
{
    std::vector<SimpleRect<int32_t> > changedRects;
    std::deque<SimpleRect<int32_t> > changedRectQueue;
    SparseGridMap<ObstacleState> uniqueChangedPoints;

    for (ChangedPoint& point : changedPoints)
    {
        if (!uniqueChangedPoints.Contains(point.xNav, point.yNav))
        {
            changedRectQueue.push_back(SimpleRect<int32_t>(point.xNav - bubbledSize, point.yNav - bubbledSize, (int32_t)bubbledSize * 2, (int32_t)bubbledSize * 2));
            uniqueChangedPoints.Set(point.xNav, point.yNav, point.newState);
        }
    }

    while (changedRectQueue.size() > 0)
    {
        SimpleRect<int32_t> activeRect = changedRectQueue.front();
        changedRectQueue.pop_front();
        bool couldMerge = false;
        for (int i = 0; i < changedRectQueue.size(); ++i)
        {
            const SimpleRect<int32_t>& rect = changedRectQueue[i];

            if (MergeRectByArea2(activeRect, rect))
            {
                // printf( "deque merge (%d,%d,%d,%d)\n", rect.StX(), rect.StY(), rect.EdX(), rect.EdY() );
                couldMerge = true;
                changedRectQueue[i] = changedRectQueue.back();
                changedRectQueue.pop_back();
                --i;
            }
        }
        if (couldMerge)
        {
            changedRectQueue.push_back(activeRect);
        }
        else
        {
            changedRects.push_back(activeRect);
        }
    }

    return changedRects;
}

std::vector<SimpleRect<int32_t> > GetChangedRectsFromChangedPoints100(std::vector<ChangedPoint>& changedPoints, int bubbledSize)
{
    std::vector<SimpleRect<int32_t> > changedRects;
    std::vector<SimpleRect<int32_t> > changedRectQueue;
    SparseGridMap<ObstacleState> uniqueChangedPoints;

    changedRectQueue.reserve(changedPoints.size());

    for (ChangedPoint& point : changedPoints)
    {
        if (!uniqueChangedPoints.Contains(point.xNav, point.yNav))
        {
            changedRectQueue.push_back(SimpleRect<int32_t>(point.xNav - bubbledSize, point.yNav - bubbledSize, (int32_t)bubbledSize * 2, (int32_t)bubbledSize * 2));
            uniqueChangedPoints.Set(point.xNav, point.yNav, point.newState);
        }
    }

    size_t pos = 0;
    while (pos < changedRectQueue.size())
    {
        SimpleRect<int32_t> activeRect = changedRectQueue[pos];
        bool couldMerge = false;
        for (int i = pos + 1; i < changedRectQueue.size(); ++i)
        {
            const SimpleRect<int32_t>& rect = changedRectQueue[i];

            if (MergeRectByArea2(activeRect, rect))
            {
                // printf( "vector merge (%d,%d,%d,%d)\n", rect.StX(), rect.StY(), rect.EdX(), rect.EdY() );
                couldMerge = true;
                changedRectQueue[i] = changedRectQueue.back();
                changedRectQueue.pop_back();
                --i;
            }
        }
        if (couldMerge)
        {
            changedRectQueue.push_back(activeRect);
        }
        else
        {
            changedRects.push_back(activeRect);
        }
        ++pos;
    }

    return changedRects;
}

using Rectangle = SimpleRect< int32_t >;
typedef RStarTree<Rectangle, 2, 16, 64>     RTree;
typedef RTree::BoundingBox			      BoundingBox;

BoundingBox bounds(int x, int y, int w, int h)
{
    BoundingBox bb;

    bb.edges[0].first  = x;
    bb.edges[0].second = x + w;

    bb.edges[1].first  = y;
    bb.edges[1].second = y + h;

    return bb;
}

BoundingBox bounds(const Rectangle& r)
{
    BoundingBox bb;

    bb.edges[0].first  = r.StX();
    bb.edges[0].second = r.EdX() + 1;

    bb.edges[1].first  = r.StY();
    bb.edges[1].second = r.EdY() + 1;

    return bb;
}

struct Visitor {
    const RTree::Leaf * res;
    bool ContinueVisiting;

    Visitor() : res(nullptr), ContinueVisiting(true) {};

    void operator()(const RTree::Leaf * const leaf)
    {
        res = leaf;
    }
};

struct LeafRemover {
    const bool ContinueVisiting;
    mutable bool mergeHappen;
    Rectangle &rect;

    LeafRemover(Rectangle& r) : ContinueVisiting(true), mergeHappen(false), rect(r) {}

    bool operator()(const RTree::Leaf * const leaf) const
    {
        auto res = MergeRectByArea2(rect, leaf->leaf);
        if( res ) mergeHappen = true;
        return res;
    }
};

std::vector<SimpleRect<int32_t> > GetChangedRectsFromChangedPoints4(std::vector<ChangedPoint>& changedPoints, int bubbledSize)
{
    std::vector<SimpleRect<int32_t> > changedRects;
    SparseGridMap<ObstacleState> uniqueChangedPoints;
    RTree tree;

    for (ChangedPoint& point : changedPoints)
    {
        if (!uniqueChangedPoints.Contains(point.xNav, point.yNav))
        {
            if( tree.GetSize() == 0 ) {
                tree.Insert( SimpleRect< int32_t >( point.xNav - bubbledSize, point.yNav - bubbledSize,
                                                    (int32_t) bubbledSize * 2, (int32_t) bubbledSize * 2 ),
                             bounds( point.xNav - bubbledSize, point.yNav - bubbledSize, (int32_t) bubbledSize * 2,
                                     (int32_t) bubbledSize * 2 ) );
            }
            else {
                auto r = Rectangle(point.xNav - bubbledSize, point.yNav - bubbledSize, (int32_t) bubbledSize * 2,
                                   (int32_t) bubbledSize * 2 );
                while( true ) {
                    LeafRemover lr(r);
                    tree.Remove(RTree::AcceptOverlapping(bounds(r)), lr );
                    if( !lr.mergeHappen ) break;
                }

                tree.Insert(r, bounds(r));

            }
            uniqueChangedPoints.Set(point.xNav, point.yNav, point.newState);
        }
    }
    printf( "tree size: %zu\n", tree.GetSize() );

    return changedRects;
}

void testQueue() {

    auto rects = GetChangedRectsFromChangedPoints( pts, BUBBLE_SIZE );

    printf( "queue rect size: %zu\n", rects.size() );
}

void testQueue2() {

    auto rects = GetChangedRectsFromChangedPoints2( pts, BUBBLE_SIZE );

    printf( "queue2 rect size: %zu\n", rects.size() );
}

void testDeque() {

    auto rects = GetChangedRectsFromChangedPoints3( pts, BUBBLE_SIZE );

    printf( "deque rect size: %zu\n", rects.size() );
}

void testVector() {

    auto rects = GetChangedRectsFromChangedPoints100( pts, BUBBLE_SIZE );

    printf( "vector rect size: %zu\n", rects.size() );
}


void testRTree() {
    auto rects = GetChangedRectsFromChangedPoints4( pts, BUBBLE_SIZE );

    printf( "rect size: %zu\n", rects.size() );
}

void init() {
    for( int i = 0; i < TESTN; ++i ) {
        pts.push_back( ChangedPoint(intRand(randEngine), intRand(randEngine), ObstacleState::DontCare) );
    }

    for( int i = 0; i < 256; ++i ) {
        pts.push_back( ChangedPoint(i, i, ObstacleState::DontCare) );
        pts.push_back( ChangedPoint(i, 0, ObstacleState::DontCare) );
        pts.push_back( ChangedPoint(0, i, ObstacleState::DontCare) );
    }

}

void diff() {
    auto rects1 = GetChangedRectsFromChangedPoints( pts, BUBBLE_SIZE );
    auto rects2 = GetChangedRectsFromChangedPoints2( pts, BUBBLE_SIZE );

    printf( "rect1 size: %zu\n", rects1.size() );
    printf( "rect2 size: %zu\n", rects2.size() );

    bool found = true;
    while( found ) {
        found = false;
        for( int i = 0; i < rects1.size(); ++i ) {
            for( int j = 0; j < rects2.size(); ++j ) {
                if( rects1[i] == rects2[j] ) {
                    rects1.erase( rects1.begin() + i );
                    rects2.erase( rects2.begin() + j );
                    found = true;
                    goto Next;
                }
            }
        }
        Next:;
    }

    printf( "rect1 size: %zu\n", rects1.size() );
    printf( "rect2 size: %zu\n", rects2.size() );

}

// 说明不同顺序，得到的结果不同
void diff2() {
    std::random_device rd;
    std::mt19937 g(rd());

    auto rects1 = GetChangedRectsFromChangedPoints( pts, BUBBLE_SIZE );
    std::shuffle( pts.begin(), pts.end(), g );
    auto rects2 = GetChangedRectsFromChangedPoints( pts, BUBBLE_SIZE );

    printf( "pts size: %zu\n", pts.size() );
    printf( "rect1 size: %zu\n", rects1.size() );
    printf( "rect2 size: %zu\n", rects2.size() );
}

int main() {

    init();
    Benchmark bm;

    auto queueTime = bm.run_once( []{ testQueue(); } );
    auto queueTime2 = bm.run_once( []{ testQueue2(); } );
    auto dequeTime = bm.run_once( []{ testDeque(); } );
    auto vectorTime = bm.run_once( []{ testVector(); } );
    auto rtreeTime = bm.run_once( []{ testRTree(); } );

    printf( "queueTime: %lld\n", queueTime );
    printf( "queueTime2: %lld\n", queueTime2 );
    printf( "dequeTime: %lld\n", dequeTime );
    printf( "vectorTime: %lld\n", vectorTime );
    printf( "rtreeTime: %lld\n", rtreeTime );

    // diff();
    // diff2();

    return 0;
}