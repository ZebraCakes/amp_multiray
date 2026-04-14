#pragma once

#include "color.h"
#include "hittable.h"
#include "vec3.h"

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
        scattered = ray(rec.pos, scatter_dir);
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
        scattered = ray(rec.pos, reflected);
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
        v3 refracted = refract(unit_direction, rec.normal, ri);

        scattered = ray(rec.pos, refracted);
        return true;
    }
};
