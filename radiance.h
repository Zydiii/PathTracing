#pragma once
#include <math.h>   
//#include "struct.h"
#include "geometry/Shape.h"

// 获得一个最大值用于判断相交
const float kInfinity = std::numeric_limits<double>::max();

// 随机数
double erand48()
{
    return (double)rand() / (double)RAND_MAX;
}

// 场景集合
Sphere spheres[] = {
    // Scene: radius, position, emission, color, material
    Sphere(1e5, Vec3d(1e5 + 1, 40.8, 81.6), Vec3d(), Vec3d(.65, .85, .92), DIFF),   // Left
    Sphere(1e5, Vec3d(-1e5 + 99, 40.8, 81.6), Vec3d(), Vec3d(.98, .73, .83), DIFF), // Rght
    Sphere(1e5, Vec3d(50, 40.8, 1e5), Vec3d(), Vec3d(.75, .75, .75), DIFF),         // Back
    Sphere(1e5, Vec3d(50, 40.8, -1e5 + 170), Vec3d(), Vec3d(), DIFF),               // Frnt
    Sphere(1e5, Vec3d(50, 1e5, 81.6), Vec3d(), Vec3d(.75, .75, .75), DIFF),         // Botm
    Sphere(1e5, Vec3d(50, -1e5 + 81.6, 81.6), Vec3d(), Vec3d(.66, .58, .85), DIFF), // Top
    Sphere(16, Vec3d(27, 16.5, 47), Vec3d(), Vec3d(.99, .99, .99) * .999, SPEC),    // Mirr
    Sphere(16.5, Vec3d(73, 16.5, 78), Vec3d(), Vec3d(.99, .99, .99), REFR),         // Glas
    Sphere(6, Vec3d(38, 6, 90), Vec3d(), Vec3d(.58, .88, .82), DIFF),               // DIFF
    Sphere(9, Vec3d(50, 9, 70), Vec3d(), Vec3d(.99, .99, .82), REFR),               // Glas
    Sphere(600, Vec3d(50, 681.6 - .27, 81.6), Vec3d(10, 10, 10), Vec3d(), DIFF)     // Lite
};

// gamma 校正
inline double clamp(double x) {
    return x < 0 ? 0 : x > 1 ? 1 : x;
}
inline int toInt(double x) { 
    return int(pow(clamp(x), 1 / 2.2) * 255 + .5); 
}

// 场景与光线求交
inline bool intersect(const Ray& r, double& t, int& id)
{
    double n = sizeof(spheres) / sizeof(Sphere), d;
    t = kInfinity;
    for (int i = int(n); i--;)
    {
        if ((spheres[i].intersect(r, d)) && d < t)
        {
            t = d;
            id = i;
        }
    }
    return t < kInfinity;
}

// 计算 radiance
Vec3d radiance(const Ray& r, int depth, unsigned short* Xi)
{
    // 判断相交
    double t;   
    int id = 0; 
    if (!intersect(r, t, id) || depth > 20)
        return Vec3d();                  
    const Sphere& obj = spheres[id]; 

    // 判断是否迭代结束
    Vec3d f = obj.color;
    double p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z; 
    if (++depth > 5)
    {
        if (erand48() < p)
            f = f * (1 / p);
        else
            return obj.emission;
    }

    // 光照计算
    Vec3d x = r.origin + r.dir * t, n = (x - obj.center).normalize(), nl = n.dot(r.dir) < 0 ? n : n * -1;
    if (obj.material == DIFF) // 漫反射
    { 
        double r1 = 2 * M_PI * erand48(), r2 = erand48(), r2s = sqrt(r2);
        Vec3d w = nl, u = ((fabs(w.x) > .1 ? Vec3d(0, 1, 0) : Vec3d(1, 0, 0)).cross(w)).normalize(), v = w.cross(u);
        Vec3d d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).normalize();
        return obj.emission + f * radiance(Ray(x, d), depth, Xi);
    }
    else if (obj.material == SPEC) // 镜面反射
        return obj.emission + f * radiance(Ray(x, r.dir - n * 2 * n.dot(r.dir)), depth, Xi);
    // 折射
    Ray reflRay(x, r.dir - n * 2 * n.dot(r.dir));
    bool into = n.dot(nl) > 0;                
    double nc = 1, nt = 1.5, nnt = into ? nc / nt : nt / nc, ddn = r.dir.dot(nl), cos2t;
    if ((cos2t = 1 - nnt * nnt * (1 - ddn * ddn)) < 0) 
        return obj.emission + f * radiance(reflRay, depth, Xi);
    Vec3d tdir = (r.dir * nnt - n * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))).normalize();
    double a = nt - nc, b = nt + nc, R0 = a * a / (b * b), c = 1 - (into ? -ddn : tdir.dot(n));
    double Re = R0 + (1 - R0) * c * c * c * c * c, Tr = 1 - Re, P = .25 + .5 * Re, RP = Re / P, TP = Tr / (1 - P);
    return obj.emission + f * (depth > 2 ?
        (erand48() < P ? radiance(reflRay, depth, Xi) * RP : radiance(Ray(x, tdir), depth, Xi) * TP)
        : radiance(reflRay, depth, Xi) * Re + radiance(Ray(x, tdir), depth, Xi) * Tr);
}