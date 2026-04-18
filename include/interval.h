#pragma once

#include "amp_def.h"

struct interval
{
    f64 min, max;

    interval() : min(+infinity), max(-infinity) {}

    interval(f64 min, f64 max) : min(min), max(max) {}

    interval(const interval& a, const interval& b)
    {
        // Create the interval tightly enclosing the two input intervals.
        min = a.min <= b.min ? a.min : b.min;
        max = a.max >= b.max ? a.max : b.max;
    }


    f64
    size() const
    {
        return max - min;
    }

    b32
    contains(f64 x) const
    {
        return min <= x && x <= max;
    }

    b32
    surrounds(f64 x) const
    {
        return min < x && x < max;
    }

    f64
    clamp(f64 x) const
    {
        if(x < min) return min;
        if(x > max) return max;
        return x;
    }

    interval
    expand(f64 delta) const
    {
        f64 padding = delta/2.0;
        return interval(min - padding, max + padding);
    }

    static const interval empty, universe;
};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);
