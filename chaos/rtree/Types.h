//
// Created by 关鑫 on 2019-08-01.
//

#pragma once

#include <cstdint>
#include <cmath>

enum class ObstacleState : int8_t
{
    DontCare = -2,
    Space = -1,
    Unknown = 0,
    Obstacle = 1
};

struct ChangedPoint
{
    int32_t xNav;
    int32_t yNav;
    ObstacleState newState;

    ChangedPoint() {}
    ChangedPoint(int32_t x_Nav, int32_t y_Nav, ObstacleState state)
            : xNav(x_Nav), yNav(y_Nav), newState(state)
    {
    }
};

template <typename T>
struct GeoVector
{
    T x;
    T y;
    GeoVector() : x(0), y(0){}
    GeoVector(const T& x, const T& y) : x(x), y(y) {}
    template <typename OtherType>
    GeoVector(const GeoVector<OtherType>& other) : x(other.x), y(other.y) {}

    template<typename PointT_1, typename PointT_2>
    void Set(const PointT_1& start, const PointT_2& end)
    {
        this->x = end.x - start.x;
        this->y = end.y - start.y;
    }
    void Set(const T &x, const T &y)
    {
        this->x = x;
        this->y = y;
    }
    template<typename V>
    T DotProduct(const GeoVector<V>& other) const
    {
        return x * other.x + y * other.y;
    }
    template<typename PointT_1, typename PointT_2>
    T DotProduct(const PointT_1& start, const PointT_2& end) const
    {
        return x * (end.x-start.x) + y * (end.y-start.y);
    }
    template<typename PointT_1, typename PointT_2>
    T DotProduct(const PointT_1 &startX, const PointT_1 &startY, const PointT_2& endX, const PointT_2& endY) const
    {
        return x * (endX-startX) + y * (endY-startY);
    }
    template<typename V>
    T CrossProduct(const GeoVector<V>& other) const
    {
        return x * other.y - y * other.x;
    }
    template<typename PointT_1, typename PointT_2>
    T CrossProduct(const PointT_1& start, const PointT_2& end) const
    {
        return x * (end.y-start.y) - y * (end.x-start.x);
    }
    template<typename PointT_1, typename PointT_2>
    T CrossProduct(const PointT_1 &startX, const PointT_1 &startY, const PointT_2& endX, const PointT_2& endY) const
    {
        return x * (endY-startY) - y * (endX-startX);
    }

    double Module() const
    {
        return sqrt(x * x + y * y);
    }

    GeoVector<T> Rotate(double angle) const
    {
        double c = cos(angle), s = sin(angle);
        T xt = x * c - y * s;
        T yt = x * s + y * c;
        return GeoVector<T>(xt, yt);
    }
    const GeoVector<T> operator+ (const GeoVector<T>& inc) const
    {
        return GeoVector<T>(x+inc.x, y+inc.y);
    }
    const GeoVector<T> operator- (const GeoVector<T>& inc) const
    {
        return GeoVector<T>(x-inc.x, y-inc.y);
    }
    const GeoVector<T> operator- () const
    {
        return GeoVector<T>(-x, -y);
    }
    const GeoVector<T>& operator+= (const GeoVector<T>& inc)
    {
        x += inc.x;
        y += inc.y;
        return *this;
    }
    const GeoVector<T>& operator-= (const GeoVector<T>& inc)
    {
        x -= inc.x;
        y -= inc.y;
        return *this;
    }
    const GeoVector<T>& operator= (const GeoVector<T>& other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }
    template<typename V>
    const GeoVector<T>& operator*= (const V multi)
    {
        x *= multi;
        y *= multi;
        return *this;
    }
    template<typename V>
    const GeoVector<T>& operator/= (const V div)
    {
        if(div != 0)
        {
            x /= div;
            y /= div;
        }
        return *this;
    }
    template<typename V>
    const GeoVector<T> operator* (V multi) const
    {
        return GeoVector<T>(x*multi, y*multi);
    }
    template<typename V>
    const GeoVector<T> operator/ (V div) const
    {
        if(div == 0)
        {
            return *this;
        }
        return GeoVector<T>(x/div, y/div);
    }
    bool operator==(const GeoVector<T>& other) const {return x == other.x && y == other.y;}
    bool operator!=(const GeoVector<T>& other) const {return x != other.x || y != other.y;}
};

template <typename T>
struct SimplePoint
{
    T x;
    T y;
    SimplePoint(){}
    SimplePoint(const T& x, const T& y) : x(x), y(y) {}
    template <typename OtherType>
    SimplePoint(const SimplePoint<OtherType>& other) : x(other.x), y(other.y) {}

    template <typename OtherType>
    OtherType Convert() const{ return OtherType(x, y); }

    void Set(T x, T y)
    {
        this->x = x;
        this->y = y;
    }
    // angle is anticlockwise radian
    SimplePoint<T> Rotate(const SimplePoint<T> &center, double angle) const
    {
        double c = cos(angle), s = sin(angle);
        T xt = c*(x - center.x) - s*(y - center.y) + center.x;
        T yt = s*(x - center.x) + c*(y - center.y) + center.y;
        return SimplePoint<T>(xt, yt);
    }

    bool operator==(const SimplePoint<T>& other) const {return x == other.x && y == other.y;}
    bool operator!=(const SimplePoint<T>& other) const {return x != other.x || y != other.y;}
    template<typename V>
    const SimplePoint<T> operator+ (const GeoVector<V>& inc) const
    {
        return SimplePoint<T>(x+inc.x, y+inc.y);
    }
    const GeoVector<T> operator-(const SimplePoint<T>& other) const
    {
        return GeoVector<T>((x-other.x), (y-other.y));
    }
    template<typename V>
    const SimplePoint<T> operator-(const GeoVector<V>& inc) const
    {
        return SimplePoint<T>(x-inc.x, y-inc.y);
    }
    template<typename V>
    const SimplePoint<T>& operator -= (const GeoVector<V>& inc)
    {
        x -= inc.x;
        y -= inc.y;
        return *this;
    }
    template<typename V>
    const SimplePoint<T>& operator += (const GeoVector<V>& inc)
    {
        x += inc.x;
        y += inc.y;
        return *this;
    }
    template<typename V>
    const SimplePoint<T>& operator = (const GeoVector<V>& other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }
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
    const T Area() const { return _width * _height; }

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

    SimpleRect<T> Expand(int32_t radius) const
    {
        SimpleRect<T> result(*this);
        result.StartPoint.x -= radius;
        result.StartPoint.y -= radius;
        result.EndPoint.x += radius;
        result.EndPoint.y += radius;
        result.UpdateRectInfo();
        return result;
    }

    template <typename OtherType>
    SimpleRect<T> Intersect(const SimpleRect<OtherType>& other) const
    {
        SimpleRect<T> result(*this);
        if(result.StartPoint.x < other.StartPoint.x)
        {
            result.StartPoint.x = other.StartPoint.x;
        }
        if(result.StartPoint.y < other.StartPoint.y)
        {
            result.StartPoint.y = other.StartPoint.y;
        }
        if(result.EndPoint.x > other.EndPoint.x)
        {
            result.EndPoint.x = other.EndPoint.x;
        }
        if(result.EndPoint.y > other.EndPoint.y)
        {
            result.EndPoint.y = other.EndPoint.y;
        }
        result.UpdateRectInfo();
        return result;
    }

    bool IsInRect(const T& x, const T& y) const
    {
        return x >= StX() && x <= EdX() && y >= StY() && y <= EdY();
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
