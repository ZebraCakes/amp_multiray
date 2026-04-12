#pragma once

#include "amp_def.h"

struct interval
{
    f64 min, max;

    interval() : min(+infinity), max(-infinity) {}

    interval(f64 min, f64 max) : min(min), max(max) {}

    f64 size() const
    {
        return max - min;
    }

    b32 contains(f64 x) const
    {
        return min <= x && x <= max;
    }

    b32 surrounds(f64 x) const
    {
        return min < x && x < max;
    }

    f64 clamp(f64 x) const
    {
        if(x < min) return min;
        if(x > max) return max;
        return x;
    }

    static const interval empty, universe;
};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);
