#pragma once

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"
#include <vector>

#include <algorithm>

struct bvh_node : public hittable
{
    shared_ptr<hittable> left;
    shared_ptr<hittable> right;

    bvh_node(hittable_list list) : bvh_node(list.objects, 0, list.objects.size())
    {
        // Creates a copy of the hittable_list passed in.
    }

    bvh_node(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end)
    {
        // Build the bounding box of the span of source objects.
        bbox = aabb::empty;
        for(size_t idx = start;
            idx < end;
            ++idx)
        {
            bbox = aabb(bbox, objects[idx]->bbox);
        }

        i32 axis = bbox.longest_axis();

        auto comparator = (axis == 0) ? box_compare_x :
                          (axis == 1) ? box_compare_y :
                                        box_compare_z;

        size_t span = end - start;

        if(span == 1)
        {
            left = right = objects[start];
        }
        else if(span == 2)
        {
            left = objects[start];
            right = objects[start + 1];
        }
        else
        {
            std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

            i32 mid = start + span/2;
            left = make_shared<bvh_node>(objects, start, mid);
            right = make_shared<bvh_node>(objects, mid, end);
        }

        ASSERT(std::isfinite(bbox.x.min) &&
               std::isfinite(bbox.y.min) &&
               std::isfinite(bbox.z.min) &&
               std::isfinite(bbox.x.max) &&
               std::isfinite(bbox.y.max) &&
               std::isfinite(bbox.z.max));
    }

    b32
    hit(const ray& r, interval ray_t, hit_record& record) const override
    {
        b32 result = false;
        if(bbox.hit(r, ray_t))
        {
            b32 hit_left = left->hit(r, ray_t, record);
            b32 hit_right = right->hit(r, interval(ray_t.min, hit_left ? record.t : ray_t.max), record);

            result = hit_left || hit_right;
        }

        return result;
    }

    static b32
    box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, i32 axis)
    {
        auto axis_interval_a = a->bbox.axis_interval(axis);
        auto axis_interval_b = b->bbox.axis_interval(axis);

        return axis_interval_a.min < axis_interval_b.min;
    }

    static b32
    box_compare_x(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
    {
        return box_compare(a, b, 0);
    }

    static b32
    box_compare_y(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
    {
        return box_compare(a, b, 1);
    }

    static b32
    box_compare_z(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
    {
        return box_compare(a, b, 2);
    }

};
