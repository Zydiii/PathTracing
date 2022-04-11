#pragma once
#include <vector>
#include <memory>
#include "../math/MathHelper.h"
#include "../geometry/Shape.h"

class Scene 
{
public:
	std::vector<std::unique_ptr<Shape>> objects;
	uint32_t num;

	// constructor
	Scene() : num(0) {}
    virtual ~Scene() {}

    // 判断光线与物体求交
    bool intersect(const Ray& r, double& tNear, const Shape*& hit) const
    {
        tNear = kInfinity;
        std::vector<std::unique_ptr<Shape>>::const_iterator iter = objects.begin();
        for (; iter != objects.end(); ++iter) {
            double t = kInfinity;
            if ((*iter)->intersect(r, t) && t < tNear) {
                hit = iter->get();
                tNear = t;
            }
        }

        return (hit != nullptr);
    }
};

class TestScene : public Scene
{
public:
    std::vector<Shape*> spheres = {
        // Planes
        new Plane(Vec3d(0, 40.8, 81.6), Vec3d(), Vec3d(.65, .85, .92), Vec3d(1, 0, 0), DIFF),   // Left
        new Plane(Vec3d(100, 40.8, 81.6), Vec3d(), Vec3d(.98, .73, .83), Vec3d(-1, 0, 0), DIFF),// Right
        new Plane(Vec3d(50, 40.8, 0), Vec3d(), Vec3d(.75, .75, .75), Vec3d(0, 0, 1), DIFF),     // Back
        new Plane(Vec3d(50, 40.8, 170), Vec3d(), Vec3d(), Vec3d(0, 0, -1), DIFF),               // Front
        new Plane(Vec3d(50, 0, 81.6), Vec3d(), Vec3d(.75, .75, .75), Vec3d(0, 1, 0), DIFF),     // Bottom
        new Plane(Vec3d(50, 81.6, 81.6), Vec3d(), Vec3d(.66, .58, .85), Vec3d(0, -1, 0), DIFF), // Top
        // Disk
        //new Disk(15, Vec3d(50, 81.6, 81.6), Vec3d(), Vec3d(10, 10, 10) * 1.5, Vec3d(0, 1, 0), DIFF),  // Light
        //new Disk(10, Vec3d(50, 50, 81.6), Vec3d(),Vec3d(.99, .99, .99), Vec3d(0, 0, 1), SPEC),        // Light
        // Spheres
        new Sphere(16, Vec3d(27, 16.5, 47), Vec3d(), Vec3d(.99, .99, .99), SPEC),               // Mirr
        new Sphere(16.5, Vec3d(73, 16.5, 78), Vec3d(), Vec3d(.99, .99, .99), REFR),             // Glas
        new Sphere(6, Vec3d(38, 6, 90), Vec3d(), Vec3d(.58, .88, .82), DIFF),                   // DIFF
        new Sphere(9, Vec3d(50, 9, 70), Vec3d(), Vec3d(.99, .99, .82), REFR),                   // Glas
        new Sphere(600, Vec3d(50, 681.6 - .27, 81.6), Vec3d(10, 10, 10), Vec3d(), DIFF)     // Lite
    };

    // constructor
    TestScene() {
        num = spheres.size();
        for (uint32_t i = 0; i < num; ++i) {
            objects.push_back(std::unique_ptr<Shape>(spheres[i]));
        }
    }
};

class RandomScene : public Scene
{
public:
    // constructor
    RandomScene() {
        num = 32;
        gen.seed(0);
        for (uint32_t i = 0; i < num; ++i) {
            //Vec3d randPos((0.5 - dis(gen)) * 10, (0.5 - dis(gen)) * 10, (0.5 + dis(gen) * 10));
            double randRadius = (0.5 + dis(gen) * 0.5);
            objects.push_back(std::unique_ptr<Shape>(new Sphere(randRadius)));
        }
    }
};