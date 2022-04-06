//#pragma once
//#define _USE_MATH_DEFINES
//#include "math/Vec.h"
//#include "geometry/Ray.h"
////#include "geometry/Shape.h"
//#include "geometry/Material.h"
//
////
//////// 光线
//////struct Ray
//////{
//////    Vec3d o, d;
//////    Ray(Vec3d o_, Vec3d d_) : o(o_), d(d_) {}
//////};
////
////// 材质类型
//////enum Refl_t
//////{
//////    DIFF,
//////    SPEC,
//////    REFR
//////};
////
//
//bool solveQuadratic(const double& a, const double& b, const double& c, double& x0, double& x1)
//{
//    double discr = b * b - 4 * a * c;
//    if (discr < 0) return false;
//    else if (discr == 0) {
//        x0 = x1 = -0.5 * b / a;
//    }
//    else {
//        double q = (b > 0) ?
//            -0.5 * (b + sqrt(discr)) :
//            -0.5 * (b - sqrt(discr));
//        x0 = q / a;
//        x1 = c / q;
//    }
//
//    return true;
//}
//
//// 球体
//struct Sphere
//{
//    double radius;  // radius
//    Vec3d center, emission, color; // position, emission, color
//    Mat_t material; // reflection type (DIFFuse, SPECular, REFRactive)
//    Sphere(double rad_, Vec3d p_, Vec3d e_, Vec3d c_, Mat_t refl_) 
//        : radius(rad_), center(p_), emission(e_), color(c_), material(refl_) {}
//    bool intersect(const Ray& r, double &t) const
//    {                   // returns distance, 0 if nohit
//        Vec3d op = center - r.origin; // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
//        double eps = 1e-4, b = op.dot(r.dir), det = b * b - op.dot(op) + radius * radius;
//        double t0, t1;
//
//        if (!solveQuadratic(r.dir.dot(r.dir), 2 * op.dot(op.dot(r.dir)), op.dot(op) - radius * radius, t0, t1))
//            return false;
//        
//        /*if (t0 > t1) std::swap(t0, t1);
//        if (t0 < eps) {
//            t0 = t1;
//            if (t0 < eps) return false;
//        }
//        t = t0;
//        return true;*/
//
//
//        if (det < 0)
//            return false;
//        else
//            det = sqrt(det);
//        if (b + det < eps)
//            return false;
//        t = b - det > eps ? b - det : b + det;
//        return true;
//    }
//    
//    bool intersect1(const Ray& ray, double& t) const
//    {
//        double t0, t1, eps = 1e-4;
//#if 0
//        // 几何解
//        Vec3d L = center - ray.origin;
//        double tca = L.dot(ray.dir);
//        double d2 = L.dot(L) - tca * tca;
//        if (d2 > radius * radius) 
//            return false;
//        double thc = sqrt(radius * radius - d2);
//        t0 = tca - thc;
//        t1 = tca + thc;
//#else
//        // 解析解
//        Vec3d L = ray.origin - center;
//        double a = ray.dir.dot(ray.dir);
//        double b = 2 * L.dot(ray.dir);
//        double c = L.dot(L) - radius * radius;
//        if (!solveQuadratic(a, b, c, t0, t1)) return false;
//#endif
//        // 需要获得较近的交点，获得 t > 0 的解表示在光线的正方向
//        if (t0 > t1) std::swap(t0, t1);
//        if (t0 < eps) {
//            t0 = t1;
//            if (t0 < eps) 
//                return false;
//        }
//        t = t0;
//        return true;
//    }
//};
//
