#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

struct hittable_list : public hittable
{
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { objects.push_back(object);}

    b32 hit(const ray& r, f64 ray_tmin, f64 ray_tmax, hit_record& record) const override
    {
        b32 result = false;
        hit_record temp_record;
        f64 closest_t = ray_tmax;

        for(const auto& object: objects)
        {
            if(object->hit(r, ray_tmin, closest_t, temp_record))
            {
                result = true;
                closest_t = temp_record.t;
                record = temp_record;
            }
        }

        return result;
    }
};
