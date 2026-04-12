#pragma once

#include "vec3.h"

#include <iostream>

using color = v3;

void
write_color(std::ostream& out, const color& pixel_color)
{
    f64 r = pixel_color.x;
    f64 g = pixel_color.y;
    f64 b = pixel_color.z;

    i32 ir = (i32)(255.999*r);
    i32 ig = (i32)(255.999*g);
    i32 ib = (i32)(255.999*b);

    out << ir << ' ' << ig << ' ' << ib << '\n';
}
