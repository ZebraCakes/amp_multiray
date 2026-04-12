#include <iostream>

#include "vec3.h"
#include "color.h"

int
main(int arg_count, char** args)
{
    // Image
    i32 width = 256;
    i32 height = 256;

    // Render
    // Header
    std::cout << "P3\n" << width << ' ' << height << "\n255\n";

    for(i32 j = 0;
        j < height;
        ++j)
    {
        std::clog << "\rScanlines remaining: " << (height - j) << ' ' << std::flush;
        for(i32 i = 0;
            i < width;
            ++i)
        {
            color pixel_color = color((f64)i/(width - 1.0), (f64)j/(height-1.0), 0);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                    \n";
}
