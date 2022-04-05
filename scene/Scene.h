#pragma once
#include <vector>
#include <memory>
#include "../geometry/Shape.h"

class Scene 
{
public:
	std::vector<std::unique_ptr<Shape>> objects;
	uint32_t num;

	// constructor
	Scene(){
        num = 32;
        gen.seed(0);
        for (uint32_t i = 0; i < num; ++i) {
            Vec3d randPos((0.5 - dis(gen)) * 10, (0.5 - dis(gen)) * 10, (0.5 + dis(gen) * 10));
            double randRadius = (0.5 + dis(gen) * 0.5);
            objects.push_back(std::unique_ptr<Shape>(new Sphere(randRadius, randPos)));
        }
	}
    ~Scene() {}
};

class testScene : public Scene
{
public:
    std::vector<Sphere*> spheres = {
        // Scene: radius, position, emission, color, material
        new Sphere(1e5, Vec3d(1e5 + 1, 40.8, 81.6), Vec3d(), Vec3d(.65, .85, .92), DIFF),   // Left
        new Sphere(1e5, Vec3d(-1e5 + 99, 40.8, 81.6), Vec3d(), Vec3d(.98, .73, .83), DIFF), // Rght
        new Sphere(1e5, Vec3d(50, 40.8, 1e5), Vec3d(), Vec3d(.75, .75, .75), DIFF),         // Back
        new Sphere(1e5, Vec3d(50, 40.8, -1e5 + 170), Vec3d(), Vec3d(), DIFF),               // Frnt
        new Sphere(1e5, Vec3d(50, 1e5, 81.6), Vec3d(), Vec3d(.75, .75, .75), DIFF),         // Botm
        new Sphere(1e5, Vec3d(50, -1e5 + 81.6, 81.6), Vec3d(), Vec3d(.66, .58, .85), DIFF), // Top
        new Sphere(16, Vec3d(27, 16.5, 47), Vec3d(), Vec3d(.99, .99, .99) * .999, SPEC),    // Mirr
        new Sphere(16.5, Vec3d(73, 16.5, 78), Vec3d(), Vec3d(.99, .99, .99), REFR),         // Glas
        new Sphere(6, Vec3d(38, 6, 90), Vec3d(), Vec3d(.58, .88, .82), DIFF),               // DIFF
        new Sphere(9, Vec3d(50, 9, 70), Vec3d(), Vec3d(.99, .99, .82), REFR),               // Glas
        new Sphere(600, Vec3d(50, 681.6 - .27, 81.6), Vec3d(10, 10, 10), Vec3d(), DIFF)     // Lite
    };

    // constructor
    testScene() {
        num = spheres.size();
        for (uint32_t i = 0; i < num; ++i) {
            objects.push_back(std::unique_ptr<Shape>(spheres[i]));
        }
    }
};