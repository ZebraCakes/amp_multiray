#pragma once

#include "vec3.h"

struct ray
{
    point3 origin;
    v3 dir;

    ray() {};

    ray(const point3& origin, const v3& direction): origin(origin), dir(direction) {}

    point3 at(f64 t) const
    {
        return origin + t*dir;
    }
};
