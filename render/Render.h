#pragma once
#include "../math/Vec.h"
#include "../math/MathHelper.h"
#include "../scene/Scene.h"

class Render {
public:
    // 计算 radiance
    static Vec3d radiance(const Ray& r, int depth, unsigned short* Xi, const Scene* scene)
    {
        // 判断相交
        double t;
        int id = 0;
        const Shape* obj = nullptr;

        // 如果深度过高直接返回，提升效率
        if (!(scene->intersect(r, t, obj)) || depth > 20)
            return Vec3d();

        // 判断是否迭代结束
        Vec3d f = obj->color;
        double p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z;
        if (++depth > 5)
        {
            if (erand48() < p)
                f = f * (1 / p);
            else
                return obj->emission;
        }

        // 光照计算
        Vec3d x = r.origin + r.dir * t, n, nl;
        obj->getSurfaceData(x, n);
        nl = n.dot(r.dir) < 0 ? n : n * -1;
        if (obj->material == DIFF) // 漫反射
        {
            double r1 = 2 * M_PI * erand48(), r2 = erand48(), r2s = sqrt(r2);
            Vec3d w = nl, u = ((fabs(w.x) > .1 ? Vec3d(0, 1, 0) : Vec3d(1, 0, 0)).cross(w)).normalize(), v = w.cross(u);
            Vec3d d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).normalize();
            return obj->emission + f * radiance(Ray(x, d), depth, Xi, scene);
        }
        else if (obj->material == SPEC) // 镜面反射
            return obj->emission + f * radiance(Ray(x, r.dir - n * 2 * n.dot(r.dir)), depth, Xi, scene);
        // 折射
        Ray reflRay(x, r.dir - n * 2 * n.dot(r.dir));
        bool into = n.dot(nl) > 0;
        double nc = 1, nt = 1.5, nnt = into ? nc / nt : nt / nc, ddn = r.dir.dot(nl), cos2t;
        if ((cos2t = 1 - nnt * nnt * (1 - ddn * ddn)) < 0)
            return obj->emission + f * radiance(reflRay, depth, Xi, scene);
        Vec3d tdir = (r.dir * nnt - n * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))).normalize();
        double a = nt - nc, b = nt + nc, R0 = a * a / (b * b), c = 1 - (into ? -ddn : tdir.dot(n));
        double Re = R0 + (1 - R0) * c * c * c * c * c, Tr = 1 - Re, P = .25 + .5 * Re, RP = Re / P, TP = Tr / (1 - P);
        return obj->emission + f * (depth > 2 ?
            (erand48() < P ? radiance(reflRay, depth, Xi, scene) * RP : radiance(Ray(x, tdir), depth, Xi, scene) * TP)
            : radiance(reflRay, depth, Xi, scene) * Re + radiance(Ray(x, tdir), depth, Xi, scene) * Tr);
    }
};