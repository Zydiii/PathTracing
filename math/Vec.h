#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

/// <summary>
/// 二维向量
/// </summary>
/// <typeparam name="T"></typeparam>
template<typename T>
class Vec2
{
public:
    T x, y;

    // constructor
    Vec2() : x(0), y(0) {}
    Vec2(T xx) : x(xx), y(xx) {}
    Vec2(T xx, T yy) : x(xx), y(yy) {}

    // operator
    Vec2 operator + (const Vec2& v) const
    {
        return Vec2(x + v.x, y + v.y);
    }
    Vec2 operator / (const T& r) const
    {
        return Vec2(x / r, y / r);
    }
    Vec2 operator * (const T& r) const
    {
        return Vec2(x * r, y * r);
    }
    Vec2& operator /= (const T& r)
    {
        x /= r, y /= r; return *this;
    }
    Vec2& operator *= (const T& r)
    {
        x *= r, y *= r; return *this;
    }
    friend std::ostream& operator << (std::ostream& s, const Vec2<T>& v)
    {
        return s << '[' << v.x << ' ' << v.y << ']';
    }
    friend Vec2 operator * (const T& r, const Vec2<T>& v)
    {
        return Vec2(v.x * r, v.y * r);
    }
};

typedef Vec2<double> Vec2d;
typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;

/// <summary>
/// 三维向量
/// </summary>
/// <typeparam name="T"></typeparam>
template<typename T>
class Vec3
{
public:
    T x, y, z;

    // constructor
    Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
    Vec3(T xx) : x(xx), y(xx), z(xx) {}
    Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}

    // operator
    Vec3 operator + (const Vec3& v) const
    {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }
    Vec3 operator - (const Vec3& v) const
    {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }
    Vec3 operator - () const
    {
        return Vec3(-x, -y, -z);
    }
    Vec3 operator * (const T& r) const
    {
        return Vec3(x * r, y * r, z * r);
    }
    Vec3 operator * (const Vec3& v) const
    {
        return Vec3(x * v.x, y * v.y, z * v.z);
    }
    Vec3& operator /= (const T& r)
    {
        x /= r, y /= r, z /= r; return *this;
    }
    Vec3& operator *= (const T& r)
    {
        x *= r, y *= r, z *= r; return *this;
    }
    friend Vec3 operator * (const T& r, const Vec3& v)
    {
        return Vec3<T>(v.x * r, v.y * r, v.z * r);
    }
    friend Vec3 operator / (const T& r, const Vec3& v)
    {
        return Vec3<T>(r / v.x, r / v.y, r / v.z);
    }

    friend std::ostream& operator << (std::ostream& s, const Vec3<T>& v)
    {
        return s << '[' << v.x << ' ' << v.y << ' ' << v.z << ']';
    }

    // math operation
    T dot(const Vec3<T>& v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }
    Vec3 cross(const Vec3<T>& v) const
    {
        return Vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    T norm() const
    {
        return x * x + y * y + z * z;
    }
    T length() const
    {
        return sqrt(norm());
    }
    Vec3& normalize()
    {
        T n = norm();
        if (n > 0) {
            T factor = 1 / sqrt(n);
            x *= factor, y *= factor, z *= factor;
        }

        return *this;
    }

    // iterator
    const T& operator [] (uint8_t i) const { return (&x)[i]; }
    T& operator [] (uint8_t i) { return (&x)[i]; }
};

typedef Vec3<double> Vec3d;
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;