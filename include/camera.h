#pragma once

#include "amp_def.h"
#include "color.h"
#include "hittable.h"
#include "vec3.h"
#include "material.h"

struct camera
{
    f64 aspect_ratio = 1.0;
    i32 image_width = 100;
    i32 image_height;
    i32 samples_per_pixel = 10;
    f64 pixel_samples_scale = 10;
    i32 max_depth = 10;

    point3 center;
    point3 pixel00_loc;
    v3 pixel_delta_u;
    v3 pixel_delta_v;

    void render(const hittable& world)
    {
        initialize();

        // Header
        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for(i32 j = 0;
            j < image_height;
            ++j)
        {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for(i32 i = 0;
                i < image_width;
                ++i)
            {
                color pixel_color = color(0, 0, 0);
                for(i32 sample = 0;
                    sample < samples_per_pixel;
                    ++sample)
                {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }

                write_color(std::cout, pixel_samples_scale*pixel_color);
            }
        }

        std::clog << "\rDone.                    \n";
    }

private:
    void initialize()
    {
        image_height = i32(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = point3(0, 0, 0);

        // Determine viewport dimensions
        f64 focal_length = 1.0;
        f64 viewport_height = 2.0;
        f64 viewport_width = viewport_height*((f64)image_width/image_height);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        v3 viewport_u = v3(viewport_width, 0, 0);
        v3 viewport_v = v3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        point3 viewport_upper_left = center - v3(0, 0, focal_length) - viewport_u/2.0 - viewport_v/2.0;
        pixel00_loc = viewport_upper_left + 0.5*(pixel_delta_u + pixel_delta_v);
    }

    ray get_ray(i32 i, i32 j) const
    {
        v3 offset = sample_square();
        point3 pixel_sample = pixel00_loc +
                                ((i + offset.x)*pixel_delta_u) +
                                ((j + offset.y)*pixel_delta_v);
        v3 ray_origin = center;
        v3 ray_direction = pixel_sample - ray_origin;
        return ray(center, ray_direction);
    }

    v3 sample_square() const
    {
        return v3(random_f64() - 0.5, random_f64() - 0.5, 0);
    }

    color ray_color(const ray& r, i32 depth, const hittable& world) const
    {
        if(depth <= 0)
        {
            return color(0,0,0);
        }
        hit_record record;
        if(world.hit(r, interval(0.001, infinity), record))
        {
            ray scattered;
            color attenuation;
            if(record.mat->scatter(r, record, attenuation, scattered))
            {
                return attenuation*ray_color(scattered, depth-1, world);
            }
            return color(0, 0, 0);
        }

        v3 unit_dir = unit_vector(r.dir);
        f64 a = 0.5*(unit_dir.y + 1.0);
        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }
};
