#pragma once

#include "vec3.h"
#include "amp_def.h"
#include "color.h"
#include "hittable.h"
#include "material.h"

struct camera
{
    f64 aspect_ratio = 1.0;
    i32 image_width = 100;
    i32 image_height;
    i32 samples_per_pixel = 10;
    f64 pixel_samples_scale = 10.0;
    i32 max_depth = 10;

    f64 fov_y = 90.0;
    point3 look_from = point3(0,0, 0);
    point3 look_at   = point3(0,0,-1);
    v3 up            = v3(0,1,0);

    f64 defocus_angle = 0.0; // Variation angle of rays through each pixel
    f64 focus_dist = 10.0;   // Distance from camera look_from point to plane of perfect focus

    v3 u, v, w;
    v3 defocus_disk_u;       // Defocus disk horizontal radius
    v3 defocus_disk_v;       // Defocus disk vertical radius

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

        center = look_from;

        // Determine viewport dimensions
        f64 theta = degrees_to_radians(fov_y);
        f64 h = std::tan(theta/2.0);
        f64 viewport_height = 2.0*h*focus_dist;
        f64 viewport_width = viewport_height*((f64)image_width/image_height);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        w = unit_vector(look_from - look_at);
        u = unit_vector(cross(up, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        v3 viewport_u = viewport_width*u; // vector across viewport horizontal edge
        v3 viewport_v = viewport_height*-v; // vector down viewport vertical edge

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        point3 viewport_upper_left = center - (focus_dist*w) - viewport_u/2.0 - viewport_v/2.0;
        pixel00_loc = viewport_upper_left + 0.5*(pixel_delta_u + pixel_delta_v);

        // Calculate the camera defocus disk basis vectors.
        f64 defocus_radius = focus_dist*std::tan(degrees_to_radians(defocus_angle/2.0));
        defocus_disk_u = u*defocus_radius;
        defocus_disk_v = v*defocus_radius;
    }

    ray get_ray(i32 i, i32 j) const
    {
        // Construct a camera ray originating from the defocus disk and directed at a randomly
        // samples point around the pixel location i, j.
        v3 offset = sample_square();
        point3 pixel_sample = pixel00_loc +
                                ((i + offset.x)*pixel_delta_u) +
                                ((j + offset.y)*pixel_delta_v);
        v3 ray_origin = (defocus_angle <= 0.0) ? center : defocus_disk_sample();
        v3 ray_direction = pixel_sample - ray_origin;
        f64 ray_time = random_f64();

        return ray(ray_origin, ray_direction, ray_time);
    }

    v3 sample_square() const
    {
        return v3(random_f64() - 0.5, random_f64() - 0.5, 0);
    }

    point3 defocus_disk_sample() const
    {
        // Returns a random point in the camera defocus disk.
        v3 pos = random_in_unit_disk();
        return center + pos[0]*defocus_disk_u + pos[1]*defocus_disk_v;
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
