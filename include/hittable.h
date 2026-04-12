#pragma once

#include "ray.h"

struct hit_record
{
    point3 pos;
    v3 normal;
    f64 t;
    b32 front_face;

    void set_face_normal(const ray& r, const v3& outward_normal)
    {
        front_face = dot(r.dir, outward_normal) < 0.0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

struct hittable
{
    virtual ~hittable() = default;

    virtual b32 hit(const ray& r, f64 ray_tmin, f64 ray_tmax, hit_record& record) const = 0;
};
