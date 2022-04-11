#pragma once
#include <random>
#include "../math/Vec.h"
#include "Material.h"
#include "Ray.h"

// 随机数
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0, 1);

/// <summary>
/// 形状基类
/// </summary>
class Shape
{
public:
    Vec3d position, color, emission;
    Mat_t material;

    // constructor
    Shape() : color(dis(gen), dis(gen), dis(gen)), emission(Vec3d()), position(dis(gen), dis(gen), dis(gen)), material(DIFF) {}
    Shape(Vec3d p, Vec3d c, Vec3d e, Mat_t mat) : position(p), color(c), emission(e), material(mat) {}
    virtual ~Shape() {}
    
    // 判断与光线相交
    virtual bool intersect(const Ray&, double&) const = 0;
    
    // 计算交点信息
    virtual void getSurfaceData(const Vec3d&, Vec3d&) const = 0;
};

/// <summary>
/// 二元一次方程求解
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="c"></param>
/// <param name="x0"></param>
/// <param name="x1"></param>
/// <returns></returns>
bool solveQuadratic(const double& a, const double& b, const double& c, double& x0, double& x1)
{
    double discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) {
        x0 = x1 = -0.5 * b / a;
    }
    else {
        double q = (b > 0) ?
            -0.5 * (b + sqrt(discr)) :
            -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }

    return true;
}

/// <summary>
/// 球体
/// </summary>
class Sphere : public Shape
{
public:
    double radius, radius2;

    // constructor
    Sphere(const double& r) : radius(r), radius2(r* r) {}
    Sphere(const double& r, const Vec3d& p, const Vec3d& e, const Vec3d& c, const Mat_t& mat) 
        : radius(r), radius2(r* r), Shape(p, c, e, mat) {}

    // 球体与光线相交
    bool intersect(const Ray& ray, double& t) const
    {
        double t0, t1;
#if 0
        // 几何解
        Vec3d L = center - ray.origin;
        double tca = L.dot(ray.dir);
        double d2 = L.dot(L) - tca * tca;
        if (d2 > radius * radius)
            return false;
        double thc = sqrt(radius * radius - d2);
        t0 = tca - thc;
        t1 = tca + thc;
#else
        // 解析解
        Vec3d L = ray.origin - position;
        double a = ray.dir.dot(ray.dir);
        double b = 2 * ray.dir.dot(L);
        double c = L.dot(L) - radius2;
        if (!solveQuadratic(a, b, c, t0, t1)) return false;
#endif
        // 需要获得较近的交点，获得 t > 0 的解表示在光线的正方向
        if (t0 > t1) std::swap(t0, t1);
        if (t0 < eps) {
            t0 = t1;
            if (t0 < eps)
                return false;
        }
        t = t0;
        return true;
    }

    /// <summary>
    /// 获得交点数据，法线、纹理坐标
    /// </summary>
    /// <param name="Phit"></param>
    /// <param name="Nhit"></param>
    /// <param name="tex"></param>
    void getSurfaceData(const Vec3d& Phit, Vec3d& Nhit) const
    {
        Nhit = (Phit - position).normalize();
        /*tex.x = (1 + atan2(Nhit.z, Nhit.x) / M_PI) * 0.5;
        tex.y = acosf(Nhit.y) / M_PI;*/
    }  
};

/// <summary>
/// 平面
/// </summary>
/// <param name="p"></param>
/// <param name="c"></param>
/// <param name="e"></param>
/// <param name="mat"></param>
/// <param name=""></param>
class Plane : public Shape {
public:
    Vec3d normal;

    // constructor
    Plane() : normal(Vec3d(dis(gen), dis(gen), dis(gen))){}
    Plane(const Vec3d& p, const Vec3d& e, const Vec3d& c, const Vec3d &n, const Mat_t& mat) 
        : Shape(p, c, e, mat), normal(n) {}

    // 光线与平面求交
    bool intersect(const Ray& ray, double& t) const {
        // (l_0 + l * t - p_0) · n = 0 => t = (p_0 - l_0) · n / (l · n)
        double denom = ray.dir.dot(normal);
        if (denom < eps) {
            Vec3d p0l0 = position - ray.origin;
            t = p0l0.dot(normal) / denom;
            return t >= 0;
        }
        return false;
    }

    /// <summary>
    /// 获得交点数据，法线
    /// </summary>
    /// <param name="Phit"></param>
    /// <param name="Nhit"></param>
    /// <param name="tex"></param>
    void getSurfaceData(const Vec3d& Phit, Vec3d& Nhit) const
    {
        Nhit = normal;
    }
};


class Disk : public Shape {
public:
    Vec3d normal;
    double radius, radius2;

    // constructor
    Disk(const double& r) : normal(Vec3d(dis(gen), dis(gen), dis(gen))), radius(dis(gen)), radius2(r* r) {}
    Disk(const double &r, const Vec3d& p, const Vec3d& e, const Vec3d& c, const Vec3d& n, const Mat_t& mat)
        : radius(r), radius2(r * r), Shape(p, e, c, mat), normal(n) {}

    // 光线与圆盘求交
    bool intersect(const Ray& ray, double& t) const {
        double denom = ray.dir.dot(normal);
        if (denom < eps) {
            Vec3d p0l0 = position - ray.origin;
            t = p0l0.dot(normal) / denom;
            if (t < 0)
                return false;
            else {
                Vec3d p = ray.origin + ray.dir * t;
                Vec3d v = p - position;
                return v.dot(v) <= radius2;
            }
        }
        return false;
    }

    /// <summary>
    /// 获得交点数据，法线
    /// </summary>
    /// <param name="Phit"></param>
    /// <param name="Nhit"></param>
    /// <param name="tex"></param>
    void getSurfaceData(const Vec3d& Phit, Vec3d& Nhit) const
    {
        Nhit = normal;
    }
};