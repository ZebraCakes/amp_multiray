#pragma once

#include <cmath>
#include <iostream>
#include "amp_def.h"

struct v3
{
    union
    {
        f64 e[3];

        struct
        {
            f64 x;
            f64 y;
            f64 z;
        };
    };

    v3() : e{0, 0, 0} {}
    v3(f64 e0, f64 e1, f64 e2) : e{e0, e1, e2} {}

    v3 operator-() const { return v3(-e[0], -e[1], -e[2]); }
    f64 operator[](int idx) const {return e[idx];}
    f64& operator[](int idx) { return e[idx];}

    v3& operator+=(const v3& vec)
    {
        e[0] += vec.e[0];
        e[1] += vec.e[1];
        e[2] += vec.e[2];

        return *this;
    }

    v3& operator*=(f64 scalar)
    {
        e[0] *= scalar;
        e[1] *= scalar;
        e[2] *= scalar;

        return *this;
    }

    v3& operator/=(f64 scalar)
    {
        return *this *= 1/scalar;
    }

    double length() const
    {
        return std::sqrt(length_squared());
    }

    double length_squared() const
    {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    static v3 random()
    {
        return v3(random_f64(), random_f64(), random_f64());
    }

    static v3 random(f64 min, f64 max)
    {
        return v3(random_f64(min, max), random_f64(min, max), random_f64(min, max));
    }
};

using point3 = v3;

inline std::ostream& operator<<(std::ostream& out, const v3& v)
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline v3 operator+(const v3& u, const v3& v)
{
    return v3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline v3 operator-(const v3& u, const v3& v)
{
    return v3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// Hadamard product
inline v3 operator*(const v3& u, const v3& v)
{
    return v3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline v3 operator*(f64 t, const v3& vec)
{
    return v3(t*vec.e[0], t*vec.e[1], t*vec.e[2]);
}

inline v3 operator*(const v3& vec, f64 t)
{
    return t*vec;
}

inline v3 operator/(const v3& vec, f64 t)
{
    return (1.0/t)*vec;
}

inline f64 dot(const v3& u, const v3& v)
{
    return u.e[0]*v.e[0] +
           u.e[1]*v.e[1] +
           u.e[2]*v.e[2];
}

inline v3 cross(const v3& u, const v3& v)
{
    return v3(u.e[1]*v.e[2] - u.e[2]*v.e[1],
              u.e[2]*v.e[0] - u.e[0]*v.e[2],
              u.e[0]*v.e[1] - u.e[1]*v.e[0]);
}

inline v3 unit_vector(const v3& vec)
{
    return vec/vec.length();
}

inline v3
random_unit_vector()
{
    while(true)
    {
        v3 pos = v3::random(-1, 1);
        f64 len_sq = pos.length_squared();
        if(1e-160 < len_sq && len_sq <= 1)
        {
            return pos / sqrt(len_sq);
        }
    }
}

inline v3
random_on_hemisphere(const v3& normal)
{
    v3 on_unit_sphere = random_unit_vector();
    if(dot(on_unit_sphere, normal) > 0.0) // in the same hemisphere/direction
    {
        return on_unit_sphere;
    }
    else
    {
        return -on_unit_sphere;
    }
}
