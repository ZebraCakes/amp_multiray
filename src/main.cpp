#include "bvh.h"
#include "material.h"
#include "vec3.h"
#include "amp_def.h"
#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include <memory>

void
bouncing_spheres()
{
    hittable_list world;

    shared_ptr<material> ground_material = std::make_shared<lambertian>(std::make_shared<checker_texture>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9)));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for(i32 a = -11;
        a < 11;
        ++a)
    {
        for(i32 b = -11;
            b < 11;
            ++b)
        {
            f64 choose_mat = random_f64();
            point3 center(a + 0.9*random_f64(), 0.2, b + 0.9*random_f64());

            if((center - point3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<material> sphere_material;

                if(choose_mat < 0.8)
                {
                    // diffuse
                    color albedo = color::random()*color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    point3 center_end = center + v3(0.0, random_f64(0, 0.5), 0.0);
                    world.add(make_shared<sphere>(center, center_end, 0.2, sphere_material));
                }
                else if(choose_mat < 0.95)
                {
                    // metal
                    color albedo = color::random(0.5, 1.0);
                    f64 fuzz = random_f64(0.0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }

            }
        }
    }

    shared_ptr<material> material_1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material_1));

    shared_ptr<material> material_2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material_2));

    shared_ptr<material> material_3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material_3));

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio      = 16.0/9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.fov_y     = 20.0;
    cam.look_from = point3(13,2,3);
    cam.look_at   = point3(0,0,0);
    cam.up        = v3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    cam.render(world);

}

void
checkered_spheres()
{
    hittable_list world;

    std::shared_ptr<texture> checker = make_shared<checker_texture>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

    world.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0,  10, 0), 10, make_shared<lambertian>(checker)));

    camera cam;

    cam.aspect_ratio      = 16.0/9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.fov_y     = 20.0;
    cam.look_from = point3(13,2,3);
    cam.look_at   = point3(0,0,0);
    cam.up        = v3(0,1,0);

    cam.defocus_angle = 0.0;

    cam.render(world);
}

int main(int arg_count, char** args)
{
    i32 selection = arg_count > 1 ? (i32)(args[1][0] - '0') : 0;

    switch(selection)
    {
        case 0:
        {
            bouncing_spheres();
        }break;
        case 1:
        {
            checkered_spheres();
        }break;
    }

}
