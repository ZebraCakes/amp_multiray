#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "vec3.h"

int
main(int arg_count, char** args)
{
    hittable_list world;

    f64 r = std::cos(pi/4.0);

    shared_ptr<material> material_left  = make_shared<lambertian>(color(0,0,1));
    shared_ptr<material> material_right = make_shared<lambertian>(color(1,0,0));
    world.objects.push_back(make_shared<sphere>(point3(-r,    0.0, -1.0),   r, material_left));
    world.objects.push_back(make_shared<sphere>(point3( r,    0.0, -1.0),   r, material_right));

    camera cam;

    cam.aspect_ratio = 16.0/9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.fov_y = 90.0;

    cam.render(world);

}
