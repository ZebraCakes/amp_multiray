#pragma once

#include "aabb.h"
#include "ray.h"
#include "interval.h"
#include <memory>

struct material;

struct hit_record
{
    point3 pos;
    v3 normal;
    std::shared_ptr<material> mat;
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
    aabb bbox;
    virtual ~hittable() = default;

    virtual b32
    hit(const ray& r, interval ray_t, hit_record& record) const = 0;

};
