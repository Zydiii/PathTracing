#pragma once
#include <math.h>   
#include "struct.h"

// �����
double erand48()
{
    return (double)rand() / (double)RAND_MAX;
}

// ��������
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

// gamma У��
inline double clamp(double x) {
    return x < 0 ? 0 : x > 1 ? 1 : x;
}
inline int toInt(double x) { 
    return int(pow(clamp(x), 1 / 2.2) * 255 + .5); 
}

// �����������
inline bool intersect(const Ray& r, double& t, int& id)
{
    double n = sizeof(spheres) / sizeof(Sphere), d, inf = t = 1e20;
    for (int i = int(n); i--;)
    {
        if ((d = spheres[i].intersect(r)) && d < t)
        {
            t = d;
            id = i;
        }
    }
    return t < inf;
}

// ���� radiance
Vec3d radiance(const Ray& r, int depth, unsigned short* Xi)
{
    // �ж��ཻ
    double t;   
    int id = 0; 
    if (!intersect(r, t, id) || depth > 20)
        return Vec3d();                  
    const Sphere& obj = spheres[id]; 

    // �ж��Ƿ��������
    Vec3d f = obj.c;
    double p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z; 
    if (++depth > 5)
    {
        if (erand48() < p)
            f = f * (1 / p);
        else
            return obj.e;
    }

    // ���ռ���
    Vec3d x = r.o + r.d * t, n = (x - obj.p).norm(), nl = n.dot(r.d) < 0 ? n : n * -1;
    if (obj.refl == DIFF) // ������
    { 
        double r1 = 2 * M_PI * erand48(), r2 = erand48(), r2s = sqrt(r2);
        Vec3d w = nl, u = ((fabs(w.x) > .1 ? Vec3d(0, 1) : Vec3d(1)).cross(w)).norm(), v = w.cross(u);
        Vec3d d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).norm();
        return obj.e + f.mult(radiance(Ray(x, d), depth, Xi));
    }
    else if (obj.refl == SPEC) // ���淴��
        return obj.e + f.mult(radiance(Ray(x, r.d - n * 2 * n.dot(r.d)), depth, Xi));
    // ����
    Ray reflRay(x, r.d - n * 2 * n.dot(r.d)); 
    bool into = n.dot(nl) > 0;                
    double nc = 1, nt = 1.5, nnt = into ? nc / nt : nt / nc, ddn = r.d.dot(nl), cos2t;
    if ((cos2t = 1 - nnt * nnt * (1 - ddn * ddn)) < 0) 
        return obj.e + f.mult(radiance(reflRay, depth, Xi));
    Vec3d tdir = (r.d * nnt - n * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))).norm();
    double a = nt - nc, b = nt + nc, R0 = a * a / (b * b), c = 1 - (into ? -ddn : tdir.dot(n));
    double Re = R0 + (1 - R0) * c * c * c * c * c, Tr = 1 - Re, P = .25 + .5 * Re, RP = Re / P, TP = Tr / (1 - P);
    return obj.e + f.mult(depth > 2 ? 
        (erand48() < P ? radiance(reflRay, depth, Xi) * RP : radiance(Ray(x, tdir), depth, Xi) * TP)
        : radiance(reflRay, depth, Xi) * Re + radiance(Ray(x, tdir), depth, Xi) * Tr);
}