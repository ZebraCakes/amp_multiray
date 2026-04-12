#pragma once

#include "interval.h"
#include "vec3.h"

#include <iostream>

using color = v3;

void
write_color(std::ostream& out, const color& pixel_color)
{
    f64 r = pixel_color.x;
    f64 g = pixel_color.y;
    f64 b = pixel_color.z;

    internal const interval intensity(0.000, 0.999);
    i32 ir = (i32)(256 * intensity.clamp(r));
    i32 ig = (i32)(256 * intensity.clamp(g));
    i32 ib = (i32)(256 * intensity.clamp(b));

    out << ir << ' ' << ig << ' ' << ib << '\n';
}
