#include <iostream>

#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"

color
ray_color(const ray& r, const hittable& world)
{
    hit_record record;
    if(world.hit(r, 0, infinity, record))
    {
        return 0.5*(record.normal + color(1, 1, 1));
    }

    v3 unit_dir = unit_vector(r.dir);
    f64 a = 0.5*(unit_dir.y + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

int
main(int arg_count, char** args)
{
    // Image
    f64 aspect_ratio = 16.0 / 9.0;
    i32 width = 400;
    i32 height = i32(width / aspect_ratio);
    height = (height < 1) ? 1 : height;

    // World
    hittable_list world;
    world.objects.push_back(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.objects.push_back(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera
    f64 focal_length = 1.0;
    f64 viewport_height = 2.0;
    f64 viewport_width = viewport_height*((f64)width/height);
    point3 camera_center = point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    v3 viewport_u = v3(viewport_width, 0, 0);
    v3 viewport_v = v3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    v3 pixel_delta_u = viewport_u / width;
    v3 pixel_delta_v = viewport_v / height;

    // Calculate the location of the upper left pixel.
    point3 viewport_upper_left = camera_center - v3(0, 0, focal_length) - viewport_u/2.0 - viewport_v/2.0;
    point3 pixel00_loc = viewport_upper_left + 0.5*(pixel_delta_u + pixel_delta_v);

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
            point3 pixel_center = pixel00_loc + (i*pixel_delta_u) + (j*pixel_delta_v);
            v3 ray_direction = pixel_center - camera_center;
            ray r = ray(camera_center, ray_direction);

            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                    \n";
}
