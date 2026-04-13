#pragma once

#include <cstdint>
#include <limits>
#include <memory>
#include <random>
#include <sys/types.h>

using u8  = uint8_t;
using u16 = uint16_t;
using u64 = uint64_t;
using u32 = uint32_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

// TODO(apeterson): When implemented in clang, use fixed width fp types
//                  https://cppreference.net/cpp/types/floating-point.html
using f32 = float;
using f64 = double;

using b32 = i32;

using bitset8  = u8;
using bitset16 = u16;
using bitset32 = u32;
using bitset64 = u64;

#define internal static
#define global static
#define local_persist static

#define kilobytes(n) (n)*1024
#define megabytes(n) kilobytes(n)*1024
#define gigabytes(n) megabytes(n)*1024

#define amp_swap(a, b, T){\
                        T temp = (a);\
                      (a) = (b);\
                      (b) = temp;\
}

#ifdef MM_SLOW
#define _ASSERT_GLUE(a, b) a ## b
#define ASSERT_GLUE(a, b) _ASSERT_GLUE(a, b)

#define ASSERT(exp) if(exp){}else{*(int*)0 = 0;}
#define STATIC_ASSERT(exp) enum{ASSERT_GLUE(g_assert_fail_, __LINE__) = 1 / (int)(!!(expr))}
#else
#define ASSERT(exp)
#define STATIC_ASSERT(exp)
#endif

// NOTE(apeterson): From rtweekend.h in the book
using std::make_shared;
using std::shared_ptr;

const f64 infinity = std::numeric_limits<f64>::infinity();
const f64 pi = 3.1415926535897932385;

inline f64
degrees_to_radians(f64 degrees)
{
    return degrees * pi/180.0;
}

inline f64
random_f64()
{
    static std::uniform_real_distribution<f64> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline f64
random_f64(f64 min, f64 max)
{
    return min + (max-min)*random_f64();
}
