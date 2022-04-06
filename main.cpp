// smallpt, a Path Tracer by Kevin Beason, 2008
// Make : g++ -O3 -fopenmp smallpt.cpp -o smallp
// Remove "-fopenmp" for g++ version < 4.2
// Usage: time ./smallpt 5000 && xv image.ppm
#include <fstream>
#include <string>
#include "radiance.h"

int main(int argc, char* argv[])
{
    // 设置图像和相机
    int w = 1024, h = 768, samps = argc == 2 ? atoi(argv[1]) / 4 : 5;
    Vec3d r, *c = new Vec3d[w * h];
    Ray cam(Vec3d(50, 52, 295.6), Vec3d(0, -0.042612, -1).normalize());
    Vec3d cx = Vec3d(w * .5135 / h, 0, 0), cy = cx.cross(cam.dir).normalize() * .5135;

    // 像素点采样计算
    #pragma omp parallel for schedule(dynamic, 1) private(r) 
    for (int y = 0; y < h; y++)
    {
        fprintf(stderr, "\rRendering (%d spp) %5.2f%%", samps * 4, 100. * y / (h - 1));
        for (unsigned short x = 0, Xi[3] = { 0, 0, y * y * y }; x < w; x++)
        {
            for (int sy = 0, i = (h - y - 1) * w + x; sy < 2; sy++)
            {
                for (int sx = 0; sx < 2; sx++, r = Vec3d())
                { 
                    for (int s = 0; s < samps; s++)
                    {
                        double r1 = 2 * erand48(), dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
                        double r2 = 2 * erand48(), dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);
                        Vec3d d = cx * (((sx + .5 + dx) / 2 + x) / w - .5) +
                            cy * (((sy + .5 + dy) / 2 + y) / h - .5) + cam.dir;
                        r = r + radiance(Ray(cam.origin + d * 140, d.normalize()), 0, Xi) * (1. / samps);
                    } 
                    c[i] = c[i] + Vec3d(clamp(r.x), clamp(r.y), clamp(r.z)) * .25;
                }
            }            
        }            
    }

    // 写文件
    std::string filename = "./result/result_" + std::to_string(samps * 4) + ".ppm";
    std::ofstream file;
    file.open(filename);
    file << "P3\n" << w << "\n" << h << "\n255\n";
    for (int i = 0; i < w * h; i++)
        file << toInt(c[i].x) << " " << toInt(c[i].y) << " " << toInt(c[i].z) << " ";
    file.close();
}
