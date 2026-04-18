#pragma once

#include "amp_def.h"
#include "color.h"
#include "hittable.h"
#include "vec3.h"
#include <cmath>

struct material
{
    virtual ~material() = default;

    virtual b32 scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const
    {
        return false;
    }
};

struct lambertian : public material
{
    color albedo;

    lambertian(const color& albedo) : albedo(albedo) {}

    b32 scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
    {
        v3 scatter_dir = rec.normal + random_unit_vector();
        if(scatter_dir.near_zero())
        {
            scatter_dir = rec.normal;
        }
        scattered = ray(rec.pos, scatter_dir, r_in.dt);
        attenuation = albedo;
        return true;
    }
};

struct metal : public material
{
    color albedo;
    f64 fuzz;

    metal(const color& albedo, f64 fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    b32 scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
    {
        v3 reflected = reflect(r_in.dir, rec.normal);
        reflected = unit_vector(reflected) + (fuzz*random_unit_vector());
        scattered = ray(rec.pos, reflected, r_in.dt);
        attenuation = albedo;
        return dot(scattered.dir, rec.normal) > 0.0;
    }

};

struct dielectric : public material
{
    f64 refraction_index;
    dielectric(f64 refraction_index) : refraction_index(refraction_index) {}

    b32 scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
    {
        attenuation = color(1.0, 1.0, 1.0);
        f64 ri = rec.front_face ? (1.0/refraction_index) : refraction_index;

        v3 unit_direction = unit_vector(r_in.dir);
        f64 cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
        f64 sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

        b32 cannot_refract = ri*sin_theta > 1.0;
        v3 direction;

        if(cannot_refract || reflectance(cos_theta, ri) > random_f64())
        {
            direction = reflect(unit_direction, rec.normal);
        }
        else
        {
            direction = refract(unit_direction, rec.normal, ri);
        }

        scattered = ray(rec.pos, direction, r_in.dt);
        return true;
    }

    static f64 reflectance(f64 cosine, f64 refraction_index)
    {
        f64 r0 = (1.0 - refraction_index) / (1.0 + refraction_index);
        r0 = r0*r0;
        return r0 + (1-r0)*std::pow((1.0 - cosine), 5.0);
    }
};
