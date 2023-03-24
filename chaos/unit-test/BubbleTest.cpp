//
// Created by 关鑫 on 2020/12/8.
//

#include <algorithm>
#include <cstdint>
#include <limits>
#include <cmath>

const int BUBBLE_SIZE = 5;
const float MAX_FLOAT = std::numeric_limits<float>::max();
const int MAP_SIZE = 1024;


template <typename T>
struct SimplePoint
{
    T x;
    T y;
    SimplePoint(): x(0), y(0){}
    SimplePoint(const T& x, const T& y) : x(x), y(y) {}
    template <typename OtherType>
    SimplePoint(const SimplePoint<OtherType>& other) : x(other.x), y(other.y) {}
};

template <typename T>
struct SimpleRect
{
    SimplePoint<T> StartPoint;
    SimplePoint<T> EndPoint;

    inline const T& StX() const { return StartPoint.x; }
    inline const T& StY() const { return StartPoint.y; }
    inline const T& EdX() const { return EndPoint.x; }
    inline const T& EdY() const { return EndPoint.y; }
    inline const T& Width() const { return _width; }
    inline const T& Height() const { return _height; }
    inline const T Area() const { return _width * _height; }
    inline const bool IsValidRect() const { return EndPoint.x >= StartPoint.x && EndPoint.y >= StartPoint.y; }

    inline void SetStX(const T& x) { StartPoint.x = x; UpdateRectInfo(); }
    inline void SetStY(const T& y) { StartPoint.y = y; UpdateRectInfo(); }
    inline void SetEdX(const T& x) { EndPoint.x = x; UpdateRectInfo(); }
    inline void SetEdY(const T& y) { EndPoint.y = y; UpdateRectInfo(); }
    inline void Set(const T& stX, const T& stY, const T& width, const T& height)
    {
        SetStX(stX);
        SetStY(stY);
        SetEdX(stX + width - 1);
        SetEdY(stY + height - 1);
        UpdateRectInfo();
    }
    inline void Set(const SimplePoint<T>& stPoint, const SimplePoint<T>& edPoint)
    {
        StartPoint = stPoint;
        EndPoint = edPoint;
        UpdateRectInfo();
    }

    void UpdateRect(const T& x, const T& y)
    {
        if (x < StX()) SetStX(x);
        if (y < StY()) SetStY(y);
        if (x > EdX()) SetEdX(x);
        if (y > EdY()) SetEdY(y);
        UpdateRectInfo();
    }
    bool IsInRect(const T& x, const T& y) const
    {
        return x >= StX() && x <= EdX() && y >= StY() && y <= EdY();
    }
    bool IsStrictlyInRect(const T& x, const T& y) const
    {
        return x > StX() && x < EdX() && y > StY() && y < EdY();
    }
    bool IsInEdge(const T& x, const T& y) const
    {
        return IsInRect(x, y) && !IsStrictlyInRect(x, y);
    }
    template <typename V>
    bool IsInsideOtherRect(const SimpleRect<V>& other) const
    {
        return StX() >= other.StX() && StY() >= other.StY() && EdX() <= other.EdX() && EdY() <= other.EdY();
    }

    bool operator==(const SimpleRect<T>& other) const
    {
        return StartPoint == other.StartPoint && EndPoint == other.EndPoint;
    }

    bool operator!=(const SimpleRect<T>& other) const
    {
        return !((*this) == other);
    }

    SimpleRect()  :
            StartPoint(0, 0), EndPoint(0, 0){ UpdateRectInfo(); };
    SimpleRect(const SimplePoint<T>& stPoint, const SimplePoint<T>& edPoint) :
            StartPoint(stPoint), EndPoint(edPoint) { UpdateRectInfo(); }
    SimpleRect(const SimplePoint<T>& stPoint, const T& width, const T& height) :
            StartPoint(stPoint), EndPoint(stPoint.x + width - 1, stPoint.y + height - 1) { UpdateRectInfo();}
    SimpleRect(const T& stX, const T& stY, const T& width, const T& height) :
            StartPoint(stX, stY), EndPoint(stX + width - 1, stY + height - 1) { UpdateRectInfo(); }
    SimpleRect(const SimpleRect<T>& another) :
            StartPoint(another.StartPoint), EndPoint(another.EndPoint) { UpdateRectInfo(); }

    template <typename OtherType>
    SimpleRect(const SimpleRect<OtherType>& other) :
            StartPoint(other.StartPoint), EndPoint(other.EndPoint){ UpdateRectInfo(); };

private:
    T _width;
    T _height;

    void UpdateRectInfo()
    {
        _width = ((EndPoint.x - StartPoint.x + 1) > 0) ? (EndPoint.x - StartPoint.x + 1) : 0;
        _height = ((EndPoint.y - StartPoint.y + 1) > 0) ? (EndPoint.y - StartPoint.y + 1) : 0;
    }
};

bool obsMap[MAP_SIZE][MAP_SIZE];
float bubbledMap[MAP_SIZE][MAP_SIZE];
float localBubbledMap[MAP_SIZE][MAP_SIZE];

void Init() {
    for (auto y = 0; y < MAP_SIZE; ++y) {
        for( auto x = 0; x < MAP_SIZE; ++x ) {
            localBubbledMap[y][x] = bubbledMap[y][x] = -1;
        }
    }
}

void CopyToLocal(const SimpleRect<int32_t>& range)
{
    for (auto y = range.StY(); y <= range.EdY(); ++y) {
        for( auto x = range.StX(); x <= range.EdX(); ++x ) {
            localBubbledMap[y][x] = bubbledMap[y][x];
        }
    }
}

void CopyFromLocal(const SimpleRect<int32_t>& range)
{
    for (auto y = range.StY(); y <= range.EdY(); ++y) {
        for( auto x = range.StX(); x <= range.EdX(); ++x ) {
            bubbledMap[y][x] = localBubbledMap[y][x];
        }
    }
}

bool IsObstacle(int x, int y)
{
    return obsMap[y][x];
}

SimpleRect<int32_t> GetRect(int x, int y)
{
    return SimpleRect<int32_t>( {x - BUBBLE_SIZE, y - BUBBLE_SIZE}, {x + BUBBLE_SIZE, y + BUBBLE_SIZE} );
}

void Bubble(const SimpleRect<int32_t>& range)
{
    // auto &mapRange = _spBubbledMap->GetRange();
    auto mapRange = SimpleRect<int32_t>( {0, 0}, {MAP_SIZE - 1, MAP_SIZE - 1} );
    SimpleRect<int32_t> expandedRange(
            {
                    std::max(range.StX() - 1, mapRange.StX()),
                    std::max(range.StY() - 1, mapRange.StY())
            },
            {
                    std::min(range.EdX() + 1, mapRange.EdX()),
                    std::min(range.EdY() + 1, mapRange.EdY())
            });

    if (!expandedRange.IsValidRect()) return;

    CopyToLocal(expandedRange);

    for (auto y = range.StY(); y <= range.EdY(); ++y)
    {
        for (auto x = range.StX(); x <= range.EdX(); ++x)
        {
            auto& cell = localBubbledMap[y][x];
            if (IsObstacle(x, y))
            {
                cell = 0;
            }
            else
            {
                cell = MAX_FLOAT;
            }
        }
    }

#define BUBBLE_ASSIGN(x, y, v) do { \
    if (expandedRange.IsStrictlyInRect((x), (y))) { \
        auto& __v = localBubbledMap[y][x]; \
        if((v) < __v) __v = (v); \
    } \
} while(0)

    for (auto y = expandedRange.StY(); y <= expandedRange.EdY(); ++y)
    {
        for (auto x = expandedRange.StX(); x <= expandedRange.EdX(); ++x)
        {
            auto& cell = localBubbledMap[y][x];

            if (0 <= cell && cell < BUBBLE_SIZE)
            {
                BUBBLE_ASSIGN(x + 1, y, cell + 1);
                BUBBLE_ASSIGN(x - 1, y + 1, cell + M_SQRT2);
                BUBBLE_ASSIGN(x, y + 1, cell + 1);
                BUBBLE_ASSIGN(x + 1, y + 1, cell + M_SQRT2);
            }
        }
    }

    for (auto y = expandedRange.EdY(); y >= expandedRange.StY(); --y)
    {
        for (auto x = expandedRange.EdX(); x >= expandedRange.StX(); --x)
        {
            auto& cell = localBubbledMap[y][x];

            if (0 <= cell && cell < BUBBLE_SIZE)
            {
                BUBBLE_ASSIGN(x - 1, y, cell + 1);
                BUBBLE_ASSIGN(x + 1, y - 1, cell + M_SQRT2);
                BUBBLE_ASSIGN(x, y - 1, cell + 1);
                BUBBLE_ASSIGN(x - 1, y - 1, cell + M_SQRT2);
            }
            if (expandedRange.IsStrictlyInRect(x, y) && cell == MAX_FLOAT)
            {
                cell = -1;
            }
        }
    }

    CopyFromLocal(expandedRange);
}

void Change( int x, int y, bool isObs )
{
    obsMap[y][x] = isObs;
    Bubble(GetRect(x, y));
}

void print(const SimpleRect<int32_t> &range)
{
    for (auto y = range.StY(); y <= range.EdY(); ++y) {
        for( auto x = range.StX(); x <= range.EdX(); ++x ) {
            if (bubbledMap[y][x] < 0) printf( "%2.1f ", bubbledMap[y][x] );
            else printf( "%3.2f ", bubbledMap[y][x] );
//            if (bubbledMap[y][x] < 0) printf( "%d_%d_%2.1f ", y, x, bubbledMap[y][x] );
//            else printf( "%d_%d_%3.2f ", y, x, bubbledMap[y][x] );
        }
        putchar( '\n' );
    }
    putchar( '\n' );
}



int main() {

    Init();

    Change(100, 100, true);
    Change(101, 100, true);
    Change(102, 102, true);
    print(SimpleRect<int32_t>( {90, 90}, {110, 110}) );

    Change(100, 100, false);
    print(SimpleRect<int32_t>( {90, 90}, {110, 110}) );

    Change(101, 100, false);
    print(SimpleRect<int32_t>( {90, 90}, {110, 110}) );

    Change(102, 102, false);
    print(SimpleRect<int32_t>( {90, 90}, {110, 110}) );

    int32_t ind = std::round(MAX_FLOAT );
    printf( "%d\n", ind );
    printf( "%f %f %f\n", MAX_FLOAT, MAX_FLOAT + 1, MAX_FLOAT + 1.42 );

    return 0;
}
