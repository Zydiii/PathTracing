#pragma once
#define _USE_MATH_DEFINES
#include "math/Vec.h"

// 光线
struct Ray
{
    Vec3d o, d;
    Ray(Vec3d o_, Vec3d d_) : o(o_), d(d_) {}
};

// 材质类型
enum Refl_t
{
    DIFF,
    SPEC,
    REFR
};

// 球体
struct Sphere
{
    double rad;  // radius
    Vec3d p, e, c; // position, emission, color
    Refl_t refl; // reflection type (DIFFuse, SPECular, REFRactive)
    Sphere(double rad_, Vec3d p_, Vec3d e_, Vec3d c_, Refl_t refl_) : rad(rad_), p(p_), e(e_), c(c_), refl(refl_) {}
    double intersect(const Ray& r) const
    {                   // returns distance, 0 if nohit
        Vec3d op = p - r.o; // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
        double t, eps = 1e-4, b = op.dot(r.d), det = b * b - op.dot(op) + rad * rad;
        if (det < 0)
            return 0;
        else
            det = sqrt(det);
        return (t = b - det) > eps ? t : ((t = b + det) > eps ? t : 0);
    }
};
