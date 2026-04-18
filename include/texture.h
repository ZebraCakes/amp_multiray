#pragma once

#include "color.h"
#include <memory>
struct texture
{
    virtual ~texture() = default;

    virtual color value(f64 u, f64 v, const point3& pos) const = 0;
};

struct solid_color : public texture
{
    color albedo;

    solid_color(const color& albedo) : albedo(albedo) {}

    solid_color(f64 red, f64 green, f64 blue) : solid_color(color(red, green, blue)) {}

    color value(f64 u, f64 v, const point3& pos) const override
    {
        return albedo;
    }
};

struct checker_texture : public texture
{
    f64 inv_scale;
    std::shared_ptr<texture> even;
    std::shared_ptr<texture> odd;

    checker_texture(f64 scale, std::shared_ptr<texture> even, shared_ptr<texture> odd): inv_scale(1.0/scale), even(even), odd(odd) {}

    checker_texture(f64 scale, const color& c1, const color& c2) : checker_texture(scale, make_shared<solid_color>(c1), make_shared<solid_color>(c2)) {}

    color
    value(f64 u, f64 v, const point3& pos) const override
    {
        i32 x = i32(std::floor(inv_scale*pos.x));
        i32 y = i32(std::floor(inv_scale*pos.y));
        i32 z = i32(std::floor(inv_scale*pos.z));

        b32 is_even = (x + y + z) % 2 == 0;

        return is_even ? even->value(u, v, pos) : odd->value(u, v, pos);
    }

};
