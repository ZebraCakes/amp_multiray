#include <cstdint>
#include <iostream>

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8 =  uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64= double;

using b32 = bool;

int
main(int arg_count, char** args)
{
    // Image
    i32 width = 256;
    i32 height = 256;

    // Render
    std::cout << "P3\n" << width << ' ' << height << "\n255\n";

    for(i32 j = 0;
        j < height;
        ++j)
    {
        for(i32 i = 0;
            i < width;
            ++i)
        {
            f64 r = f64(i) / (width-1);
            f64 g = f64(j) / (height-1);
            f64 b = 0.0;

            i32 ir = (i32)(255.999*r);
            i32 ig = (i32)(255.999*g);
            i32 ib = (i32)(255.999*b);


            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
}
