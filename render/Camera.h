#pragma once
#include "../math/Vec.h"

/// <summary>
/// Па»ъ
/// </summary>
struct Camera
{
    Vec3d origin, dir;
    Camera(Vec3d o_, Vec3d d_) : origin(o_), dir(d_) {}
};