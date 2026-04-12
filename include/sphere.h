#pragma once

#include "hittable.h"
#include "vec3.h"
#include <cmath>

struct sphere : public hittable
{
    point3 center;
    f64 radius;

    sphere(const point3& center, f64 radius) : center(center), radius(std::fmax(0, radius)) {}

    b32 hit(const ray& r, interval ray_t, hit_record& record) const override
    {
        v3 oc = center - r.origin;
        f64 a = r.dir.length_squared();
        f64 h = dot(r.dir, oc);
        f64 c = oc.length_squared() - radius*radius;

        f64 discriminant = h*h - a*c;
        if(discriminant < 0.0)
        {
            return false;
        }

        f64 sqrt = std::sqrt(discriminant);

        f64 root = (h - sqrt) / a;
        if(!ray_t.surrounds(root))
        {
            root = (h + sqrt) / a;
            if(!ray_t.surrounds(root))
            {
                return false;
            }
        }

        record.t = root;
        record.pos = r.at(record.t);
        v3 outward_normal = (record.pos - center) / radius;
        record.set_face_normal(r, outward_normal);

        return true;
    }
};
