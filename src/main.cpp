#include <iostream>

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "vec3.h"

int
main(int arg_count, char** args)
{
    hittable_list world;
    world.objects.push_back(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.objects.push_back(make_shared<sphere>(point3(0, -100.5, -1), 100));

    camera cam;

    cam.aspect_ratio = 16.0/9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.render(world);

}
