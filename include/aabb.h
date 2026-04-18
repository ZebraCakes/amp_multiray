#pragma once

#include "vec3.h"
#include "interval.h"
#include "ray.h"

struct aabb
{
    interval x, y, z;

    aabb() {} // The default AABB is empty, since intervals are empty by default.

    aabb(const interval& x, const interval& y, const interval& z): x(x), y(y), z(z) {}

    aabb(const point3& a, const point3& b)
    {
        // Treat the two points a and b as extrema for the bounding box, so we don't require a
        // particular minimum/maximum coordinate order.
        x = (a[0] <= b[0] ) ? interval(a[0], b[0]) : interval(b[0], a[0]);
        y = (a[1] <= b[1] ) ? interval(a[1], b[1]) : interval(b[1], a[1]);
        z = (a[2] <= b[2] ) ? interval(a[2], b[2]) : interval(b[2], a[2]);
        ASSERT(std::isfinite(x.min) &&
               std::isfinite(y.min) &&
               std::isfinite(z.min) &&
               std::isfinite(x.max) &&
               std::isfinite(y.max) &&
               std::isfinite(z.max));
    }

    aabb(const aabb& a, const aabb& b)
    {
        x = interval(a.x, b.x);
        y = interval(a.y, b.y);
        z = interval(a.z, b.z);
        ASSERT(std::isfinite(x.min) &&
               std::isfinite(y.min) &&
               std::isfinite(z.min) &&
               std::isfinite(x.max) &&
               std::isfinite(y.max) &&
               std::isfinite(z.max));
    }


    const interval&
    axis_interval(i32 n) const
    {
        if(n == 1) return y;
        if(n == 2) return z;
        return x;
    }

    b32
    hit(const ray& r, interval ray_t) const
    {
        const point3& ray_orig = r.origin;
        const v3&     ray_dir  = r.dir;

        for(i32 axis = 0;
            axis < 3;
            ++axis)
        {
            const interval& ax = axis_interval(axis);
            const f64 ad_inv = 1.0 / ray_dir[axis];

            f64 t0 = (ax.min - ray_orig[axis])*ad_inv;
            f64 t1 = (ax.max - ray_orig[axis])*ad_inv;

            if(t0 < t1)
            {
                if(t0 > ray_t.min) ray_t.min = t0;
                if(t1 < ray_t.max) ray_t.max = t1;
            }
            else
            {
                if(t1 > ray_t.min) ray_t.min = t1;
                if(t0 < ray_t.max) ray_t.max = t0;
            }

            if(ray_t.max <= ray_t.min)
            {
                return false;
            }
        }

        return true;
    }

    i32
    longest_axis() const
    {
        // Returns the index of the longest axis of the bounding box.
        if(x.size() > y.size())
        {
            return x.size() > z.size() ? 0 : 2;
        }
        else
        {
            return y.size() > z.size() ? 1 : 2;
        }
    }

    static const aabb empty;
    static const aabb universe;
};

const aabb aabb::empty =    aabb(interval::empty,    interval::empty,    interval::empty);
const aabb aabb::universe = aabb(interval::universe, interval::universe, interval::universe);
