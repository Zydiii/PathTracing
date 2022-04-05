#pragma once
#include "../math/Vec.h"

/// <summary>
/// π‚œﬂ
/// </summary>
struct Ray
{
    Vec3d origin, dir;
    Ray(Vec3d o_, Vec3d d_) : origin(o_), dir(d_) {}
};