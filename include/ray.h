#pragma once

#include "vec3.h"

struct ray
{
    point3 origin;
    v3 dir;
    f64 dt;

    ray() {};

    ray(const point3& origin, const v3& direction, f64 time): origin(origin), dir(direction), dt(time) {}

    ray(const point3& origin, const v3& direction): origin(origin), dir(direction), dt(0.0) {}

    point3 at(f64 t) const
    {
        return origin + t*dir;
    }
};
