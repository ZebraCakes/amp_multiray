#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "vec3.h"

int
main(int arg_count, char** args)
{
    hittable_list world;

    shared_ptr<material> material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    shared_ptr<material> material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    shared_ptr<material> material_left   = make_shared<dielectric>(1.50);
    shared_ptr<material> material_bubble = make_shared<dielectric>(1.00/1.50);
    shared_ptr<material> material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.objects.push_back(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.objects.push_back(make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.objects.push_back(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.objects.push_back(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.4, material_bubble));
    world.objects.push_back(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    camera cam;

    cam.aspect_ratio = 16.0/9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.fov_y = 20.0;
    cam.look_from = point3(-2,2,1);
    cam.look_at   = point3(0,0,-1);
    cam.up        = v3(0,1,0);

    cam.defocus_angle = 10.0;
    cam.focus_dist    = 3.4;

    cam.render(world);

}
